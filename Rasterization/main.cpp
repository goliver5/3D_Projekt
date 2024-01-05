#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <vector>

#include "imGUI\imconfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_dx11.h"
#include "imGUI\imgui_internal.h"
#include "imGUI\imstb_rectpack.h"
#include "imGUI\imstb_textedit.h"
#include "imGUI\imstb_truetype.h"
#include "imGUI\imgui_impl_win32.h"

#include "WindowHelper.h"
#include "Camera.h"
#include "D3D11Helper.h"
#include "PipelineHelper.h"
#include "ConstantBuffer.h"
#include "ConstantBufferNew.h"
#include "LightCBuffer.h"
#include "BufferTypes.h"
#include "OBJParser.h"
#include "objectStruct.h"
#include "SceneObject.h"
#include <chrono>
#include "Camera.h"
#include "DefferedRendering.h"
#include "ParticleSystem.h"
#include "ShadowMapping.h"
#include "Tesselator.h"
#include "CubeMapping.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "FrustumCulling.h"
#include "ObjParserHelper.h"
#include "StructuredBufferSpotLights.h"

void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout,
	ID3D11Buffer* vertexBuffer, ID3D11SamplerState* sampler, 
	ID3D11ComputeShader* cShader, ID3D11UnorderedAccessView* backBufferUAV,
	ConstantBufferNew<VPMatrix> &VPcBuffer, Camera &camera, std::vector<SceneObject*>& testScene,
	DefferedRendering*& defferedRenderer, ParticleSystem & particleSystem, ID3D11ComputeShader*& particleComputeShader,
	ID3D11PixelShader*& pixelParticleShader, ID3D11GeometryShader*& geometryShader, ShadowMapping &shadows,
	Tesselator& tesselator, CubeMapping& cubeMapping, bool renderParticles, bool changeCamera, bool WireFrameMode, Camera& secondCamera,
	std::vector<SpotLight>& spotlights, DirectionalLight& dirLight)
{
	float clearColour[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	tesselator.setWireFrameMode(immediateContext, WireFrameMode);


	//camera.setVSBuffer(immediateContext);
	

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	immediateContext->RSSetViewports(1, &viewport);
	camera.setVSBuffer(immediateContext);
	immediateContext->PSSetSamplers(0, 1, &sampler);


	immediateContext->VSSetShader(vShader, nullptr, 0);

	//shadow prepass
	//immediateContext->OMSetRenderTargets(1, &rtv, nullptr);
	
	if (changeCamera)
	{
		camera.setVSBuffer(immediateContext);
	}
	else
	{
		secondCamera.setVSBuffer(immediateContext);
	}
	shadows.shadowFirstPass(immediateContext, testScene, spotlights);
	//sätter om viewprojection matrisen

	//camera.setVSBuffer(immediateContext); GÖR REDAN DET ETT PAR RADER UPP MEN KANSKE GÖRA OM DET EFTER SHADOWS DRAW CALL

	//sätter skuggans camera till main kamera
	if (changeCamera)
	{
		camera.setVSBuffer(immediateContext);
	}
	else
	{
		secondCamera.setVSBuffer(immediateContext);
	}

	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	//ta väck för forward rendering och måste ta väck defferedrenderer
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	// deferred rendering börjar första pass (light pass)
	defferedRenderer->firstPass(immediateContext, dsView);

	shadows.setSRV(immediateContext);
	tesselator.setTesselatorState(immediateContext);
	camera.setHullShaderCameraPos(0, 1, immediateContext);
	//int start = 0;
	//for (int i = 0; i < spotlights.size(); i++)
	//{
	//	spotlights[i].setbuffer(i, 1, immediateContext);
	//	start++;
	//}
	shadows.setCameraBuffer(immediateContext);
	for (int i = 0; i < testScene.size(); i++)
	{
		testScene[i]->draw(immediateContext);
	}
	shadows.setSRVNull(immediateContext);
	immediateContext->HSSetShader(nullptr, nullptr, 0);
	immediateContext->DSSetShader(nullptr, nullptr, 0);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//clear innan
	//immediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	//deferred rendering andra pass för ljuset
	ID3D11RenderTargetView* nullRtv = nullptr;
	immediateContext->OMSetRenderTargets(1, &nullRtv, nullptr);
	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &backBufferUAV, nullptr);
	defferedRenderer->lightPass(immediateContext);
	spotlights[0].setbuffer(0, 1, immediateContext);
	spotlights[1].setbuffer(1, 1, immediateContext);
	spotlights[2].setbuffer(2, 1, immediateContext);
	camera.csSetCameraPosition(immediateContext);
	dirLight.setbuffer(4, 1, immediateContext);
	immediateContext->Dispatch(32, 32, 1);
	/*defferedRenderer->clearRenderTargets(immediateContext);*/

	ID3D11Buffer* buffer = nullptr;
	//immediateContext->CSSetConstantBuffers(0, 1, &buffer);
	//immediateContext->CSSetConstantBuffers(1, 1, &buffer);
	//immediateContext->CSSetConstantBuffers(4, 0, &buffer);

	ID3D11UnorderedAccessView* nullUav = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &nullUav, nullptr);

	defferedRenderer->clearTemp(immediateContext);

	cubeMapping.firstPass(immediateContext, testScene, particleSystem, camera, geometryShader, pixelParticleShader, vShader, inputLayout, renderParticles);
	//utanför funktionen så att imgui och cubemappen också renderas
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	//sätter kamerans position till pixelshadern
	camera.setPSCameraPosition(immediateContext);
	camera.setVSBuffer(immediateContext);
	//camera.setviewProjectionLightVertexShader(0, 1, immediateContext);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	//immediateContext->PSSetSamplers(0, 1, &sampler);

	if (changeCamera)
	{
		camera.setVSBuffer(immediateContext);
		cubeMapping.drawCube(immediateContext);
	}
	else
	{
		secondCamera.setVSBuffer(immediateContext);
	}



	if (renderParticles == true)
	{
		//partiklesystemets draw call
		immediateContext->GSSetShader(geometryShader, nullptr, 0);
		immediateContext->PSSetShader(pixelParticleShader, nullptr, 0);
		immediateContext->CSSetShader(particleComputeShader, nullptr, 0);
		particleSystem.draw(immediateContext, camera);
		particleSystem.dispatchParticles(immediateContext, camera);
	}
	//varning för nästa draw call detta fixar
	ID3D11ShaderResourceView* nullrsv = nullptr;
	immediateContext->PSSetShaderResources(0, 1, &nullrsv);
	immediateContext->PSSetShaderResources(1, 1, &nullrsv);

}

void ImguiFunction(bool &renderParticles, bool &changeCamera, Camera& camera, bool& WireFrameMode, ParticleSystem& pSystem, FrustumCulling& fCulling, DirectionalLight& dirLight,
	std::vector<SpotLight>& spotlights)
{
	static const char* items[]{ "Spot light 1", "Spot light 2" , "Spot light 3" };
	static int selectedItem = 0;
	float spotLightColorTEMP[3] = { };
	float spotLightPosition[3] = { };

	DirectX::XMFLOAT3 camPos;
	float dirLightColor[3]{ dirLight.getColor().x,dirLight.getColor().y,dirLight.getColor().z };
	DirectX::XMStoreFloat3(&camPos, camera.getcameraPosition());

	float particleSize = pSystem.getParticleSize();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	
	ImGui::NewFrame();
	{
		bool begun = ImGui::Begin("Screen Modifiers");
		if (begun)
		{
			ImGui::Text("camera Position X: %d Y: %d Z: %d", (int)camPos.x, (int)camPos.y, (int)camPos.z);
			ImGui::Checkbox("RenderParticles", &renderParticles);
			ImGui::Checkbox("ChangeCamera", &changeCamera);
			ImGui::Checkbox("wireFrameMode", &WireFrameMode);
			//ImGui::Checkbox("Im lazy", &rotation);
			ImGui::SliderFloat("ParticleSize", &particleSize, 0.0f, 1.0f);
			ImGui::ColorEdit3("Direction Light Color ", dirLightColor);

			ImGui::Combo("Select Light", &selectedItem, items, size(items));

			spotLightColorTEMP[0] = spotlights[selectedItem].getColor().x;
			spotLightColorTEMP[1] = spotlights[selectedItem].getColor().y;
			spotLightColorTEMP[2] = spotlights[selectedItem].getColor().z;
			spotLightPosition[0] = spotlights[selectedItem].getPosition().x;
			spotLightPosition[1] = spotlights[selectedItem].getPosition().y;
			spotLightPosition[2] = spotlights[selectedItem].getPosition().z;

			ImGui::DragFloat3("SpotLight Position", spotLightPosition, 0.3f, 0.5f, 0.5f);

			ImGui::ColorEdit3("SpotLight Color ", spotLightColorTEMP);	
		}
		ImGui::End();
	}

	pSystem.setParticleSize(particleSize);
	dirLight.setColor(dirLightColor[0], dirLightColor[1], dirLightColor[2]);

	spotlights[selectedItem].setPosition(spotLightPosition[0], spotLightPosition[1], spotLightPosition[2]);
	spotlights[selectedItem].setColor(spotLightColorTEMP[0], spotLightColorTEMP[1], spotLightColorTEMP[2]);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	const UINT WIDTH = 1024;
	const UINT HEIGHT = 1024;
	HWND window;

	bool renderParticles = true;
	bool changeCamera = true;
	bool WireFrameMode = false;


	auto start = std::chrono::system_clock::now();

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return-1;
	}

	ID3D11Device* device;//								released
	ID3D11DeviceContext* immediateContext;//			released
	IDXGISwapChain* swapChain;//						released
	ID3D11RenderTargetView* rtv;//						released
	ID3D11Texture2D* dsTexture;//						released
	ID3D11DepthStencilView* dsView;//					released
	D3D11_VIEWPORT viewport;
	ID3D11GeometryShader* geometryShader;//				released
	ID3D11VertexShader* vShader;//						released
	ID3D11PixelShader* pShader;//						released
	ID3D11PixelShader* pixelParticleShader;//			released
	ID3D11InputLayout* inputLayout;//					released
	ID3D11Buffer* vertexBuffer;//						released

	ID3D11SamplerState* sampler;//						released

	ID3D11ComputeShader *cShader;//						released
	ID3D11ComputeShader* particleComputeShader;//		released
	ID3D11UnorderedAccessView *backBufferUAV;//			released

	ID3D11Buffer* lightBuffer;//						released
	LightCBuffer lightClass;
	
	ConstantBufferNew<WMatrix> constantBufferNew;
	ConstantBufferNew<VPMatrix> VPcBuffer;
	ConstantBufferNew<VPMatrix> secondCameraBuffer;
	Camera camera;
	Camera secondCamera;


	DirectionalLight dirLight;
	std::vector<SpotLight> spotlights;
	const UINT NROFSPOTLIGHTS = 3;


	//static const int NROFTEXTURES = 4;
	//ID3D11ShaderResourceView* textureSRVs[NROFTEXTURES];

	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRVs[2];
	//std::vector<ID3D11ShaderResourceView*> textureSRVs;

	DefferedRendering* wow = new DefferedRendering(WIDTH, HEIGHT);
	ParticleSystem particleSystem;
	Tesselator tesselator;
	ShadowMapping shadowMapping(WIDTH, HEIGHT);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport, backBufferUAV))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	if (!particleSystem.initiateParticleSystem(device, immediateContext))
	{
		std::cerr << "Failed to Particle System!" << std::endl;
		return -1;
	}

	wow->initGBuffers(device);

	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, sampler, cShader, geometryShader, particleComputeShader, pixelParticleShader))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	if (!lightClass.setLightCBuffer(device, lightBuffer, immediateContext))
	{
		std::cerr << "Failed to set lightbuffer!\n";
		return -1;
	}

	/*if (!SetupTextures(device, textureSRVs))
	{
		return -1;
	}*/
	std::vector<objectStruct> allObjectData;
	
	ObjParserHelper(allObjectData, device);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, immediateContext);

	//tempObject.rotateObject(1.5, 0.0f, 0.0f);
	std::vector<SceneObject*> testScene;

	std::vector<ID3D11ShaderResourceView*> tempDEBUGGING;

	SceneObject cubeObject(device, immediateContext, allObjectData[1]);

	CubeMapping cubeMapping(cubeObject, WIDTH, HEIGHT);

	FrustumCulling frustumCulling;

	StructuredBufferSpotLights strSpotLights;
	if (!strSpotLights.initialize(device, immediateContext)) {
		return -1;
	}
	strSpotLights.addSpotLight(device);

	for (int i = 0; i < 10; i++)
	{
		testScene.push_back(new SceneObject(device, immediateContext, allObjectData[1]));
		testScene[i]->setPosition(i * 5.0f, 0.0f, 0.0f);
	}
	//test


	//roterande kub
	testScene.push_back(new SceneObject(device, immediateContext, allObjectData[2]));
	//golvet
	testScene.push_back(new SceneObject(device, immediateContext, allObjectData[0]));
	testScene.push_back(new SceneObject(device, immediateContext, allObjectData[0]));
	testScene.push_back(new SceneObject(device, immediateContext, allObjectData[0]));
	testScene[3]->setPosition(8.0f, 4.0f, 9.0f);
	testScene[2]->setPosition(3.0f, 0.0f, 10.0f);
	testScene[1]->setGroundPos();
	

	//testScene.push_back(new SceneObject(device, immediateContext, allObjectData[2]));
	//testScene[4]->setPosition(10.0f, 0.0f, 0.0f);


	if (!shadowMapping.initiateShadows(device, immediateContext)) return -1;
	if (!tesselator.initiateTesselator(device, immediateContext)) return -1;
	if (!cubeMapping.initialize(device, immediateContext))return -1;

	for (int i = 0; i < NROFSPOTLIGHTS; i++)
	{
		spotlights.push_back(SpotLight());
		if (!spotlights[i].initialize(device, immediateContext)) return -1;
	}
	spotlights[1].setPosition(0.0f, 20.0f, 10.0);
	spotlights[2].setPosition(0.0f, 20.0f, -10.0);
	



	if (!dirLight.initialize(device, immediateContext)) return -1;

	constantBufferNew.Initialize(device, immediateContext);
	camera.initializeCamera(device, immediateContext, VPcBuffer);
	//camera.setRotation(0.0f, 1.0f, 0.0f);
	secondCamera.initializeCamera(device, immediateContext, secondCameraBuffer);
	secondCamera.setPosition(0.0f, 90.0f, 0.0f);
	secondCamera.setRotation(1.5f, 0.0f, 0.0f);
	MSG msg = {};

	frustumCulling.culling(testScene,camera);
	frustumCulling.frustumCheck(camera);
	std::vector<SceneObject*> currentScene;

	//currentScene = frustumCulling.getScene();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > 1.0f / 120.0f)
		{
			//cBuffer.updateConstantBuffer(device, constantBuffer, immediateContext);
			start = std::chrono::system_clock::now();
			//tempObject.tempUpdate();
			camera.update(immediateContext);

			if (GetAsyncKeyState('R'))
			{
				currentScene.push_back(new SceneObject(device, immediateContext, allObjectData[0]));
			}
			//for (int i = 2; i < testScene.size(); i++)
			//{
			//	testScene[i].tempUpdate();
			//}

			//roterande kub
			//testScene[0]->tempUpdate();
			//testScene[0]->flyLeft(); 

			for (int i = 0; i < testScene.size(); i++)
			{
				testScene[i]->update();
			}
			
			//frustumCulling.clearTree();
			frustumCulling.frustumCheck(camera);
			currentScene = frustumCulling.getScene();

			//lightClass.updateLightCBuffer(device, lightBuffer, immediateContext);
			//cBuffer.updateConstantBuffer(device, constantBuffer, immediateContext);

			Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, sampler,
				cShader, backBufferUAV, VPcBuffer, camera, currentScene, wow, particleSystem, particleComputeShader, pixelParticleShader,
				geometryShader, shadowMapping, tesselator, cubeMapping, renderParticles, changeCamera, WireFrameMode, secondCamera, spotlights, dirLight);

			ImguiFunction(renderParticles, changeCamera, camera, WireFrameMode, particleSystem, frustumCulling, dirLight, spotlights);
			
		}
		swapChain->Present(0, 0);
	}

	cShader->Release();
	backBufferUAV->Release();
	sampler->Release();

	lightBuffer->Release();

	wow->noMoreMemoryLeaks();
	delete wow;

	//for (int i = 0; i < NROFTEXTURES; i++)
	//{
	//	textureSRVs[i]->Release();
	//}
	for (int i = 0; i < testScene.size(); i++)
	{
		testScene[i]->noMemoryLeak();
	}
	for (int i = 0; i < testScene.size(); i++)
	{
		delete testScene[i];
	}

	
	pixelParticleShader->Release();
	vertexBuffer->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();
	geometryShader->Release();
	particleComputeShader->Release();

	for (int i = 0; i < allObjectData.size(); i++)
	{
		allObjectData[i].noMemoryLeaks();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	return 0;
}