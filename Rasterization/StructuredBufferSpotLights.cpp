#include "StructuredBufferSpotLights.h"
StructuredBufferSpotLights::StructuredBufferSpotLights()
{
}
bool StructuredBufferSpotLights::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool value = buffer.Initialize(device, deviceContext);
	const int index = buffer.getSizeOfVector() - 1;
	buffer.getData(index).position = DirectX::XMFLOAT3(0.0f, 20.0f, 35.0f);
	buffer.getData(index).color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer.getData(index).direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	buffer.getData(index).attenuation = DirectX::XMFLOAT3(0.7f, 0.2f, 0.0f);
	buffer.getData(index).cone = 50.0f;
	buffer.getData(index).range = 50.0f;
	buffer.applyData();
	return value;
}

void StructuredBufferSpotLights::setColor(float r, float g, float b, const int index)
{
	buffer.getData(index).color = DirectX::XMFLOAT3(r, g, b);
}

void StructuredBufferSpotLights::setPosition(float x, float y, float z, const int index)
{
	buffer.getData(index).position = DirectX::XMFLOAT3(x, y, z);
	buffer.applyData();
}

void StructuredBufferSpotLights::setDirection(float x, float y, float z, const int index)
{
	buffer.getData(index).direction = DirectX::XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT3 StructuredBufferSpotLights::getColor(const int index)
{
	return buffer.getData(index).color;
}

DirectX::XMFLOAT3 StructuredBufferSpotLights::getPosition(const int index)
{
	return buffer.getData(index).position;
}

void StructuredBufferSpotLights::setbufferCS(const int toRegister, ID3D11DeviceContext* immediateContext)
{
	buffer.applyData();
	buffer.BindToCS(toRegister);
}

void StructuredBufferSpotLights::addSpotLight(ID3D11Device* device)
{
	spotLightData data;
	data.position = DirectX::XMFLOAT3(0.0f, 20.0f, 35.0f);
	data.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	data.attenuation = DirectX::XMFLOAT3(0.7f, 0.2f, 0.0f);
	data.cone = 50.0f;
	data.range = 50.0f;
	buffer.addData(data, device);
	
}
