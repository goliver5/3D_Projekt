#pragma once
#include <d3d11.h>
#include <string>
#include <fstream>
#include "Camera.h"

class ShadowMapping
{
private:
	//f�r att kunna skapa och �ndra view projection buffer fr�n ljuset
	Camera camera;

	UINT width;
	UINT height;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11VertexShader* vertexShadowShader; //created
	ID3D11ShaderResourceView* shadowSrv; //created
	ID3D11Texture2D* texture;
	ID3D11SamplerState* shadowSampler;

	bool initiateDepthStencils(ID3D11Device* device);
	bool initiateShadowShader(ID3D11Device* device, std::string& vShaderByteCode);
	bool initiateSrv(ID3D11Device* device);
	bool initiateShadowSampler(ID3D11Device* device);
public:
	ShadowMapping(UINT width, UINT height);
	~ShadowMapping();

	bool initiateShadows(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void shadowFirstPass(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
};