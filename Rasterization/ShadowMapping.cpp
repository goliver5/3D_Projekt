#include "ShadowMapping.h"
bool ShadowMapping::initiateDepthStencils(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;

	HRESULT hr = device->CreateDepthStencilView(this->texture, &depthStencilViewDesc, &depthStencilView);

	if (FAILED(hr)) return false;

	return true;
}

bool ShadowMapping::initiateShadowShader(ID3D11Device* device, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/VertexShadowShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		//std::cerr << "Could not open vertex shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShadowShader);

	if (FAILED(hr))
	{
		//std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}
	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();

	return true;
}

bool ShadowMapping::initiateSrv(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 4;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;// ???????? kanske ändras
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc{};
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = 1;
	shaderDesc.Texture2DArray.ArraySize = 4;
	shaderDesc.Texture2DArray.FirstArraySlice = 0;
	hr = device->CreateShaderResourceView(this->texture, &shaderDesc, &shadowSrv);

	if (FAILED(hr)) return false;

	return true;
}

bool ShadowMapping::initiateShadowSampler(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &shadowSampler);

	if (FAILED(hr)) return false;

	return true;
}

ShadowMapping::ShadowMapping(UINT width, UINT height)
	:width(width),height(height)
{
	this->depthStencilView = nullptr;
	this->shadowSrv = nullptr;
	this->vertexShadowShader = nullptr;
	this->shadowSampler = nullptr;
	this->texture = nullptr;
}

ShadowMapping::~ShadowMapping()
{
	depthStencilView->Release();
	vertexShadowShader->Release();
	shadowSrv->Release();
	shadowSampler->Release();
	texture->Release();
}

bool ShadowMapping::initiateShadows(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	std::string vShaderByteCode;

	if (!initiateShadowShader(device, vShaderByteCode))		return false;
	if (!initiateSrv(device))								return false;
	if (!initiateDepthStencils(device))						return false;
	if (!initiateShadowSampler(device))						return false;

	camera.initializeCamera(device, immediateContext, VPBuf);

	camera.setPosition(0.0f, 5.0f, -3.0f);
	camera.setRotation(0.5f, 0.f, 0.f);

	return true;
}

void ShadowMapping::shadowFirstPass(ID3D11DeviceContext* immediateContext, std::vector<SceneObject*>& sceneObjects)
{
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediateContext->VSSetShader(vertexShadowShader, nullptr, 0);

	ID3D11PixelShader* nullPShader = nullptr;
	immediateContext->PSSetShader(nullPShader, nullptr, 0u);

	immediateContext->OMSetRenderTargets(0, nullptr, depthStencilView);
	//immediateContext->OMSetDepthStencilState(depthStencilState, 0);
	immediateContext->PSSetSamplers(1, 1, &shadowSampler);
	//immediateContext->PSSetShaderResources(1, 1, &shadowSrv);
	//camera.setVSBuffer(immediateContext);

	//camera.ShadowUpdateDebug(immediateContext);

	camera.setPosition(0.0f, 5.0f, -3.0f);
	camera.setRotation(0.5f, 0.f, 0.f);

	camera.setviewProjectionLightVertexShader(2, 1, immediateContext);

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->draw(immediateContext);
	}

	//används för test sätter camera bufferns viewprojection till shadows camera
	//camera.setVSBuffer(immediateContext);

	//immediateContext->PSSetShaderResources(1, 1, &shadowSrv);

	ID3D11DepthStencilState* nullDState = nullptr;
	//immediateContext->OMSetDepthStencilState(nullDState, 0);
}

void ShadowMapping::setSRV(ID3D11DeviceContext* immediateContext)
{
	immediateContext->PSSetShaderResources(1, 1, &shadowSrv);
}

void ShadowMapping::setSRVNull(ID3D11DeviceContext* immediateContext)
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	immediateContext->PSSetShaderResources(1, 1, &nullSRV);
}

void ShadowMapping::setCameraBuffer(ID3D11DeviceContext* immediateContext)
{
	camera.setviewProjectionLightVertexShader(2, 1, immediateContext);
}

void ShadowMapping::setShadowToCurrentCamera(ID3D11DeviceContext* immediateContext)
{
	camera.setviewProjectionLightVertexShader(1, 1, immediateContext);
}

