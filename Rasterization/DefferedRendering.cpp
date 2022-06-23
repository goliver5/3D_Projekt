#include "DefferedRendering.h"

DefferedRendering::DefferedRendering(UINT windowWidth, UINT windowHeight)
	:HEIGHT(windowHeight), WIDTH(windowWidth)
{
}

bool DefferedRendering::initGBuffers()
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;


	return false;
}

bool DefferedRendering::firstPass(ID3D11Device* device, ID3D11DeviceContext*& immediateContext)
{
	return false;
}
