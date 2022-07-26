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
	ID3D11VertexShader* vertexShadowShader;
	ID3D11ShaderResourceView* shadowSrv;
	ID3D11Texture2D* texture;

	bool initiateDepthStencilState(ID3D11Device* device);
	bool initiateShadowShader(ID3D11Device* device, std::string& vShaderByteCode);
	bool initiateSrv(ID3D11Device* device);
public:
	ShadowMapping(UINT width, UINT height);

	bool initiateShadows(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void shadowFirstPass(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
};