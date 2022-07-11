#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	//20 partiklar
	float temp = 0.1;
	for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		std::vector<float> particlePositions;
		particlePositions.push_back(temp);//x
		particlePositions.push_back(temp);//y
		particlePositions.push_back(temp);//z
		temp *= i;

		particles.push_back(particlePositions);

		temp = temp + 0.01;
	}
	
}

HRESULT ParticleSystem::CreateInputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, _countof(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &particleInputLayout);

	return !FAILED(hr);
}

bool ParticleSystem::initiateVertexShader(ID3D11Device* device, std::string &vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/particleVertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open vertex shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &particleVertexShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}
	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();

	return true;
}

ParticleSystem::~ParticleSystem()
{
	uav->Release();
	vBuffer->Release();
}

bool ParticleSystem::initiateParticleSystem(ID3D11Device* device)
{
	std::string vShaderByteCode;

	if (!initiateVertexShader(device, vShaderByteCode))
	{
		return false;
	}

	if (!CreateInputLayout(device, vShaderByteCode))
	{
		return false;
	}

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
	if (FAILED(hr))	return false;

	hr = device->CreateUnorderedAccessView(vBuffer, &uavDesc, &uav);
	if (FAILED(hr))	return false;

	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	identity = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&identityMatrix.getData().world, identity);



	return true;
}

void ParticleSystem::draw(ID3D11DeviceContext*& immediateContext, Camera& camera)
{
	UINT stride = sizeof(Particles);
	UINT offset = 0;
	int size = particles.size();
	immediateContext->VSSetShader(particleVertexShader, nullptr, 0);
	immediateContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->IASetInputLayout(particleInputLayout);

	//immediateContext->GSSetShader()
	DirectX::XMStoreFloat3(&constantBuffer.getData().position, camera.getcameraPosition());

	immediateContext->CSSetUnorderedAccessViews(0, 1, &uav,nullptr);
	//immediateContext->VSSetConstantBuffers(0, 1, identityMatrix.getReferenceOf());
	immediateContext->GSSetConstantBuffers(0, 1, constantBuffer.getReferenceOf());

	immediateContext->Draw(size, 0);
	immediateContext->GSSetShader(nullptr, nullptr, 0);

}


