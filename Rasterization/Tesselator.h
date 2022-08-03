#pragma once
#include <d3d11.h>

class Tesselator
{
private:
	ID3D11RasterizerState* rasterizerDefault;
	ID3D11RasterizerState* wireFrameState;

	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainShader;

	bool wireFrameMode;

	bool initiateHullShader(ID3D11Device* device);
	bool initiateDomainShader(ID3D11Device* device);
	bool initiateRasterizerStates(ID3D11Device* device);
public:
	Tesselator();
	~Tesselator();

	bool initiateTesselator(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void setTesselatorState(ID3D11DeviceContext* immediateContext);
	void setWireFrameMode(ID3D11DeviceContext* immediateContext, bool state);
	bool getCurrenRasterizerState();
};