#pragma once
#include <d3d11.h>
#include <vector>
#include <fstream>

#include "Camera.h"
#include "SceneObject.h"
#include "ParticleSystem.h"

class CubeMapping
{
private:
	const UINT WIDTH;
	const UINT HEIGHT;

	Camera camera;
	ConstantBufferNew<VPMatrix> VPBuf;
	ConstantBufferNew<cameraPosition> cameraPositionBuffer;
	std::vector<DirectX::XMFLOAT3> cameraRotation;

	SceneObject *cubeObject;
	
	ID3D11ShaderResourceView* cubeSrv;
	ID3D11RenderTargetView* cubeRtv[6];
	ID3D11DepthStencilView* depthStencilView;
	ID3D11PixelShader* SamplepixelShader;//2 st gammal + ny för mappa cubemap xmpi/2 ninetydegrees
	ID3D11PixelShader* drawCubePixelShader;
	ID3D11SamplerState* cubeSampler;


	//texture
	//en pixelshader
	bool setupPSShader(ID3D11Device* device);
	//depthStencil
	bool setupDepthStencil(ID3D11Device* device);
	//rtv srv
	bool setupRTVandSRV(ID3D11Device* device);
	//skapa sceneObject
	//camerabuffern för position = objektetsposition
	//clear depthstencil för varje draw call
	bool setupSampler(ID3D11Device* device);

public:
	CubeMapping(SceneObject& obj, UINT WIDTH, UINT HEIGHT);
	~CubeMapping();

	bool initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	void firstPass(ID3D11DeviceContext* immediateContext, std::vector<SceneObject*> &sceneObjects, ParticleSystem& particleSystem, Camera& mainCamera, 
		ID3D11GeometryShader* geometryShader, ID3D11PixelShader* pixelParticleShader, ID3D11VertexShader* vShader, ID3D11InputLayout* inputLayout, bool renderParticles);
	void drawCube(ID3D11DeviceContext* immediateContext);
};