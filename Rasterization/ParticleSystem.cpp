#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	//20 partiklar
	float temp = 0.1;
	for (int i = 0; i < 20; i++)
	{
		std::vector<float> particlePositions;
		particlePositions.push_back(temp);//x
		particlePositions.push_back(temp);//y
		particlePositions.push_back(temp);//z

		particles.push_back(particlePositions);

		temp = temp + 0.01;
	}
	
}

bool ParticleSystem::initiateParticleSystem(ID3D11Device* device)
{

	D3D11_BUFFER_DESC bufferDesc = {
	bufferDesc.ByteWidth = sizeof(float[3])* this->particles.size(), //byte storlek
	bufferDesc.Usage = D3D11_USAGE_DEFAULT,
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS,
	bufferDesc.CPUAccessFlags = 0,
	bufferDesc.MiscFlags = 0,
	bufferDesc.StructureByteStride = 0
	};

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT,
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER,

	uavDesc.Buffer = {
	uavDesc.Buffer.FirstElement = 0,
	uavDesc.Buffer.NumElements = std::size(particles) * 3,//hur många partiklar
	uavDesc.Buffer.Flags = 0
	}
	};

	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = particles.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vBuffer);


	//skapa unordered access view-
	//skapa vertexbuffern


	hr = device->CreateUnorderedAccessView(vBuffer, &uavDesc, &uav);


	return true;
}
