#include "DefferedRendering.h"

DefferedRendering::DefferedRendering(UINT windowWidth, UINT windowHeight)
	:HEIGHT(windowHeight), WIDTH(windowWidth)
{
}

DefferedRendering::~DefferedRendering()
{
	//for (int i = 0; i < GBUFFER_COUNT; i++)
	//{
	//	gBufferRTV[i]->Release();
	//	gBufferSRV[i]->Release();
	//	textures[i]->Release();
	//}
}

void DefferedRendering::noMoreMemoryLeaks()
{
	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		gBufferRTV[i]->Release();
		gBufferSRV[i]->Release();
		textures[i]->Release();
	}
}

bool DefferedRendering::initGBuffers(ID3D11Device* device)
{
	//börjar med texturer
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr;

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		hr = device->CreateTexture2D(&textureDesc, NULL, &textures[i]);
		//gör en check med hr och return false om den inte e gucci
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		hr = device->CreateRenderTargetView(textures[i], &rtvDesc, &gBufferRTV[i]);
		//gör en check med hr och return false om den inte e gucci
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		hr = device->CreateShaderResourceView(textures[i], &srvDesc, &gBufferSRV[i]);
		//gör en check med hr och return false om den inte e gucci
	}
	

	return true;
}

void DefferedRendering::firstPass(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView)
{
	immediateContext->OMSetRenderTargets(GBUFFER_COUNT, gBufferRTV, dsView);
	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColor);
	}
}

void DefferedRendering::lightPass(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->CSSetShaderResources(0, GBUFFER_COUNT, gBufferSRV);
}
