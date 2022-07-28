#pragma once
#include <d3d11.h>

class Tesselator
{
private:
	ID3D11RasterizerState* rasterizerDefault;
	ID3D11RasterizerState* wireFrameState;

	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainShader;

	bool initiateHullShader(ID3D11DeviceContext* immediateContext, ID3D11HullShader* hullShader);
	bool initiateDomainShader(ID3D11DeviceContext* immediateContext, ID3D11DomainShader* domainShader);
	bool initiateRasterizerStates(ID3D11DeviceContext* immediateContext, ID3D11RasterizerState* rasterizerDefault, ID3D11RasterizerState* wireFrameState);
public:
	Tesselator();
	~Tesselator();

	bool initiateTesselator(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

};