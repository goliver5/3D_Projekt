#pragma once

#include "Camera.h"
#include "ConstantBufferNew.h"

#include <d3d11.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//struct Particles
//{
//	float positions[3];
//
//	Particles(const std::vector<float> position)
//	{
//		for (int i = 0; i < position.size(); i++)
//		{
//			positions[i] = position[i];
//		}
//	}
//};


const UINT NUMBER_OF_PARTICLES = 200;

class ParticleSystem
{
private:

	/*std::vector<Particles> particles;*/
	std::vector<DirectX::XMFLOAT3> particles;
	ID3D11UnorderedAccessView* uav;
	ID3D11Buffer* vBuffer;
	ID3D11VertexShader* particleVertexShader;
	ID3D11InputLayout* particleInputLayout;

	//kamerans position
	ConstantBufferNew<cameraForwardUpvector> cameraBuffer;
	//identites matrisen
	ConstantBufferNew<WMatrix> identityMatrix;
	//particle information
	ConstantBufferNew<particleInformation> particleInfo;
protected:
	HRESULT CreateInputLayout(ID3D11Device* device, const std::string& vShaderByteCode);
	bool initiateVertexShader(ID3D11Device* device, std::string& vShaderByteCode);
	

public:

	ParticleSystem();
	~ParticleSystem();

	bool initiateParticleSystem(ID3D11Device* device, ID3D11DeviceContext*& immediateContext);

	void updateParticleInformation();

	void draw(ID3D11DeviceContext* immediateContext, Camera& camera);
};