#pragma once
#include <d3d11.h>
#include <string>
#include <fstream>
#include "SceneObject.h"
#include "Camera.h"
#include "ConstantBufferNew.h"
#include "SpotLight.h"

class ShadowMapping
{
private:
	//för att kunna skapa och ändra view projection buffer från ljuset
	Camera camera[4];

	UINT width;
	UINT height;

	ID3D11DepthStencilView* depthStencilView[4];
	ID3D11VertexShader* vertexShadowShader; //created
	ID3D11ShaderResourceView* shadowSrv; //created
	ID3D11Texture2D* texture;
	ID3D11SamplerState* shadowSampler;

	ConstantBufferNew<VPMatrix> VPBuf;
	ConstantBufferNew<VPMatrix> VPBuf2;
	ConstantBufferNew<VPMatrix> VPBuf3;
	ConstantBufferNew<VPMatrix> VPBuf4;







	bool initiateDepthStencils(ID3D11Device* device);
	bool initiateShadowShader(ID3D11Device* device, std::string& vShaderByteCode);
	bool initiateSrv(ID3D11Device* device);
	bool initiateShadowSampler(ID3D11Device* device);
public:
	ShadowMapping(UINT width, UINT height);
	~ShadowMapping();

	bool initiateShadows(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void shadowFirstPass(ID3D11DeviceContext* immediateContext, std::vector<SceneObject*> &sceneObjects, std::vector<SpotLight>& spotlights);
	void setSRV(ID3D11DeviceContext* immediateContext);
	void setSRVNull(ID3D11DeviceContext* immediateContext);
	void setCameraBuffer(ID3D11DeviceContext* immediateContext);
	void setShadowToCurrentCamera(ID3D11DeviceContext* immediateContext);
};