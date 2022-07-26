#include "ShadowMapping.h"
bool ShadowMapping::initiateDepthStencilState(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

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
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(hr)) return false;
	return true;
}

ShadowMapping::ShadowMapping(UINT width, UINT height)
	:width(width),height(height)
{
	depthStencilState = nullptr;
	this->depthStencilView = nullptr;
	this->shadowSrv = nullptr;
	this->vertexShadowShader = nullptr;
}

bool ShadowMapping::initiateShadows(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	std::string vShaderByteCode;

	if (!initiateShadowShader(device, vShaderByteCode)) return false;
	if (!initiateSrv(device)) return false;
	if (!initiateDepthStencilState(device)) return false;
	
	return true;
}

void ShadowMapping::shadowFirstPass(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	immediateContext->PSSetShader(nullptr, nullptr, 0u);
}
