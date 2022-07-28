#include "Tesselator.h"

bool Tesselator::initiateHullShader(ID3D11DeviceContext* immediateContext, ID3D11HullShader* hullShader)
{
	return false;
}

bool Tesselator::initiateDomainShader(ID3D11DeviceContext* immediateContext, ID3D11DomainShader* domainShader)
{
	return false;
}

bool Tesselator::initiateRasterizerStates(ID3D11DeviceContext* immediateContext, ID3D11RasterizerState* rasterizerDefault, ID3D11RasterizerState* wireFrameState)
{
	return false;
}

Tesselator::Tesselator()
{
}

Tesselator::~Tesselator()
{
	rasterizerDefault->Release();
	wireFrameState->Release();
	domainShader->Release();
	hullShader->Release();
}

bool Tesselator::initiateTesselator(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	if (!initiateDomainShader(immediateContext, domainShader)) return false;
	if (!initiateHullShader(immediateContext, hullShader)) return false;
	if (!initiateRasterizerStates(immediateContext, rasterizerDefault, wireFrameState)) return false;

	return true;
}
