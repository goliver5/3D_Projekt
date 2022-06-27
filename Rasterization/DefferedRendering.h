#pragma once
#include<d3d11.h>

const UINT GBUFFER_COUNT = 3;

class DefferedRendering
{
private:
	const UINT WIDTH;
	const UINT HEIGHT;

	ID3D11Texture2D* textures[GBUFFER_COUNT]{ nullptr };
	ID3D11ShaderResourceView* gBufferSRV[GBUFFER_COUNT]{ nullptr };
	ID3D11RenderTargetView* gBufferRTV[GBUFFER_COUNT]{ nullptr };

public:
	DefferedRendering(UINT windowWidth, UINT windowHeight);

	bool initGBuffers(ID3D11Device* device);
	bool firstPass(ID3D11Device* device, ID3D11DeviceContext*& immediateContext);
};