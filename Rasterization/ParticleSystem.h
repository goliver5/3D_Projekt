#pragma once
#include <d3d11.h>
#include <vector>

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

public:

	ParticleSystem();

	bool initiateParticleSystem(ID3D11Device* device);

	void draw();
};