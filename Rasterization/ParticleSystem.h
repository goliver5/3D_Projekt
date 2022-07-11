#pragma once
#include <d3d11.h>
#include <vector>
#include "Camera.h"
#include "ConstantBufferNew.h"
#include <string>
#include <fstream>
#include <iostream>

struct Particles
{
	float positions[3];

	Particles(const std::vector<float> position)
	{
		for (int i = 0; i < position.size(); i++)
		{
			positions[i] = position[i];
		}
	}
};

const UINT NUMBER_OF_PARTICLES = 1000;

class ParticleSystem
{
private:

	std::vector<Particles> particles;
	ID3D11UnorderedAccessView* uav;
	ID3D11Buffer* vBuffer;
	ID3D11VertexShader* particleVertexShader;
	ID3D11InputLayout* particleInputLayout;

	//kamerans position
	ConstantBufferNew<position> constantBuffer;
	//identites matrisen
	ConstantBufferNew<WMatrix> identityMatrix;
protected:
	HRESULT CreateInputLayout(ID3D11Device* device, const std::string& vShaderByteCode);
	bool initiateVertexShader(ID3D11Device* device, std::string& vShaderByteCode);
	

public:

	ParticleSystem();
	~ParticleSystem();

	bool initiateParticleSystem(ID3D11Device* device);

	void draw(ID3D11DeviceContext*& immediateContext, Camera& camera);
};