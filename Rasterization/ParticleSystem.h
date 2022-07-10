#pragma once
#include <d3d11.h>
#include <vector>
#include "Camera.h"
#include "ConstantBufferNew.h"

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

class ParticleSystem
{
private:

	std::vector<Particles> particles;
	ID3D11UnorderedAccessView* uav;
	ID3D11Buffer* vBuffer;

	//kamerans position
	ConstantBufferNew<position> constantBuffer;
	//identites matrisen
	ConstantBufferNew<WMatrix> identityMatrix;
public:

	ParticleSystem();
	~ParticleSystem();

	bool initiateParticleSystem(ID3D11Device* device);

	void draw(ID3D11DeviceContext*& immediateContext, Camera& camera);
};