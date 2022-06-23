#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <vector>

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
#include "CreateTextures.h"
#include "DefferedRendering.h"

void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout,
	ID3D11Buffer* vertexBuffer, ID3D11SamplerState* sampler, 
	ID3D11Buffer* lightConstantBuffer, ID3D11ComputeShader* cShader, ID3D11UnorderedAccessView* backBufferUAV,
	ConstantBufferNew<VPMatrix> &VPcBuffer, Camera &camera, SceneObject &tempObj, std::vector<SceneObject>& testScene)
{
	float clearColour[4]{ 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	camera.setVSBuffer(immediateContext);
	immediateContext->PSSetConstantBuffers(0, 1, &lightConstantBuffer);

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	//immediateContext->PSSetShaderResources(0, 1, &textureSRV);
	immediateContext->PSSetSamplers(0, 1, &sampler);
	tempObj.draw(immediateContext);

	for (int i = 0; i < testScene.size(); i++)
	{
		testScene[i].draw(immediateContext);
	}

	ID3D11RenderTargetView* nullRtv = nullptr;
	immediateContext->OMSetRenderTargets(1, &nullRtv, nullptr);
	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &backBufferUAV, nullptr);

	immediateContext->Dispatch(32, 32, 1);

	ID3D11UnorderedAccessView* nullUav = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &nullUav, nullptr);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	HWND window;

	auto start = std::chrono::system_clock::now();

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return-1;
	}

	ID3D11Device* device;//					released
	ID3D11DeviceContext* immediateContext;//released
	IDXGISwapChain* swapChain;//			released
	ID3D11RenderTargetView* rtv;//			released
	ID3D11Texture2D* dsTexture;//			released
	ID3D11DepthStencilView* dsView;//		released
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;//			released
	ID3D11PixelShader* pShader;//			released
	ID3D11InputLayout* inputLayout;//		released
	ID3D11Buffer* vertexBuffer;//			released

	ID3D11SamplerState* sampler;//			released

	ID3D11ComputeShader *cShader;//			released
	ID3D11UnorderedAccessView *backBufferUAV;//released

	ID3D11Buffer* lightBuffer;//			released
	LightCBuffer lightClass;
	
	ConstantBufferNew<WMatrix> constantBufferNew;
	ConstantBufferNew<VPMatrix> VPcBuffer;
	Camera camera;

	static const int NROFTEXTURES = 2;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRVs[2];
	ID3D11ShaderResourceView* textureSRVs[NROFTEXTURES];
	//std::vector<ID3D11ShaderResourceView*> textureSRVs;

	DefferedRendering* wow = new DefferedRendering(WIDTH, HEIGHT);

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport, backBufferUAV))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, sampler, cShader))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	if (!lightClass.setLightCBuffer(device, lightBuffer, immediateContext))
	{
		std::cerr << "Failed to set lightbuffer!\n";
		return -1;
	}

	if (!SetupTextures(device, textureSRVs))
	{
		return -1;
	}

	SceneObject tempObject(device, immediateContext, textureSRVs[1]);
	std::vector<SceneObject> testScene;

	constantBufferNew.Initialize(device, immediateContext);
	camera.initializeCamera(device, immediateContext, VPcBuffer);

	MSG msg = {};

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
			tempObject.tempUpdate();
			camera.update();

			if (GetAsyncKeyState('K'))
			{
				testScene.push_back(SceneObject(device, immediateContext, textureSRVs[0]));
			}
			for (int i = 0; i < testScene.size(); i++)
			{
				testScene[i].tempUpdate();
			}
		}
		lightClass.updateLightCBuffer(device, lightBuffer, immediateContext);
		//cBuffer.updateConstantBuffer(device, constantBuffer, immediateContext);

		Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, sampler,
			lightBuffer, cShader, backBufferUAV, VPcBuffer, camera, tempObject, testScene);
		swapChain->Present(0, 0);
	}

	cShader->Release();
	backBufferUAV->Release();
	sampler->Release();

	lightBuffer->Release();

	for (int i = 0; i < NROFTEXTURES; i++)
	{
		textureSRVs[i]->Release();
	}
	for (int i = 0; i < testScene.size(); i++)
	{
		testScene[i].noMemoryLeak();
	}
	tempObject.noMemoryLeak();

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

	return 0;
}