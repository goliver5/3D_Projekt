#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	//20 partiklar
	//float temp = 0.1;
	//for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
	//{
	//	std::vector<float> particlePositions;
	//	particlePositions.push_back((float)cos(i + 1) / (float)NUMBER_OF_PARTICLES);//x
	//	particlePositions.push_back(5.0f);//y
	//	particlePositions.push_back((float)sin(i + 1) / (float)NUMBER_OF_PARTICLES);//z
	//	//temp *= i;

	//	//particles.push_back(particlePositions);

	//	//temp = temp + 0.01;
	//	particles.push_back(particlePositions);
	//}

	for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		particles.push_back({ (float)cos(i + 1) / (float)NUMBER_OF_PARTICLES, 2.0f, (float)sin(i + 1) / (float)NUMBER_OF_PARTICLES });
	}
	
}

HRESULT ParticleSystem::CreateInputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, std::size(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &particleInputLayout);

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
	particleInputLayout->Release();
	particleVertexShader->Release();
}

bool ParticleSystem::initiateParticleSystem(ID3D11Device* device, ID3D11DeviceContext*& immediateContext)
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
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3)* this->particles.size(), //byte storlek
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
	uavDesc.Buffer.NumElements = particles.size() * 3,//hur många partiklar
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

	identityMatrix.Initialize(device, immediateContext);
	cameraBuffer.Initialize(device, immediateContext);

	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	identity = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&identityMatrix.getData().world, identity);

	particleInfo.Initialize(device, immediateContext);

	identityMatrix.applyData();
	//cameraBuffer.applyData();

	return true;
}

void ParticleSystem::updateParticleInformation()
{
	particleInfo.getData().time += (1.0f / 120.0f);
	particleInfo.applyData();
}

void ParticleSystem::draw(ID3D11DeviceContext* immediateContext, Camera& camera)
{
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	int size = particles.size();
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->IASetInputLayout(particleInputLayout);
	immediateContext->VSSetShader(particleVertexShader, nullptr, 0);
	immediateContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

	//immediateContext->GSSetShader()
	cameraBuffer.getData() = camera.particleTempCamera(immediateContext);
	cameraBuffer.getData().SIZE = 10.012f;
	cameraBuffer.applyData();

	immediateContext->VSSetConstantBuffers(0, 1, identityMatrix.getReferenceOf());
	camera.setGSViewProjectionBuffer(immediateContext);
	immediateContext->GSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());

	immediateContext->Draw(size, 0);
	//tar väck från pipeline
	immediateContext->GSSetShader(nullptr, nullptr, 0);

	ID3D11Buffer* tempBuffer = nullptr;
	immediateContext->IASetVertexBuffers(0, 1, &tempBuffer, &stride, &offset);

	immediateContext->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
	immediateContext->CSSetConstantBuffers(0, 1, particleInfo.getReferenceOf());

	updateParticleInformation();
	
	immediateContext->Dispatch(50, 1, 1);

	ID3D11UnorderedAccessView* tempUav = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &tempUav, nullptr);
}


