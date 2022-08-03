#include "Tesselator.h"

bool Tesselator::initiateHullShader(ID3D11Device* device)
{
	return true;
}

bool Tesselator::initiateDomainShader(ID3D11Device* device)
{
	return true;
}

bool Tesselator::initiateRasterizerStates(ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC desc;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;

	HRESULT hr = device->CreateRasterizerState(&desc, &wireFrameState);
	if (FAILED(hr)) return false;

	D3D11_RASTERIZER_DESC descdefault;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;

	hr = device->CreateRasterizerState(&desc, &rasterizerDefault);
	if (FAILED(hr)) return false;
	return true;
}

Tesselator::Tesselator()
{
	wireFrameMode = false;
}

Tesselator::~Tesselator()
{
	rasterizerDefault->Release();
	wireFrameState->Release();
	//domainShader->Release();
	//hullShader->Release();
}

bool Tesselator::initiateTesselator(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	if (!initiateDomainShader(device)) return false;
	if (!initiateHullShader(device)) return false;
	if (!initiateRasterizerStates(device)) return false;

	return true;
}

void Tesselator::setTesselatorState(ID3D11DeviceContext* immediateContext)
{
	immediateContext->HSSetShader(hullShader, nullptr, 0);
	immediateContext->DSSetShader(domainShader, nullptr, 0);
	immediateContext->RSSetState(rasterizerDefault);
}

void Tesselator::setWireFrameMode(ID3D11DeviceContext* immediateContext, bool state)
{
	wireFrameMode = state;
	if (state)
	{
		immediateContext->RSSetState(wireFrameState);
	}
	else
	{
		immediateContext->RSSetState(rasterizerDefault);
	}
}

bool Tesselator::getCurrenRasterizerState()
{
	return wireFrameMode;
}
