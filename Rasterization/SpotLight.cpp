#include "SpotLight.h"
SpotLight::SpotLight()
{
}
bool SpotLight::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool value = buffer.Initialize(device, deviceContext);

	buffer.getData().position = DirectX::XMFLOAT3(0.0f, 20.0f, 35.0f);
	buffer.getData().color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer.getData().direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	buffer.getData().attenuation = DirectX::XMFLOAT3(0.7f, 0.2f, 0.0f);
	buffer.getData().cone = 50.0f;
	buffer.getData().range = 50.0f;
	buffer.applyData();
	return value;
}

void SpotLight::setColor(float r, float g, float b)
{
	buffer.getData().color = DirectX::XMFLOAT3(r, g, b);
}

void SpotLight::setPosition(float x, float y, float z)
{
	buffer.getData().position = DirectX::XMFLOAT3(x, y, z);
	buffer.applyData();
}

void SpotLight::setDirection(float x, float y, float z)
{
	buffer.getData().direction = DirectX::XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT3 SpotLight::getColor()
{
	return buffer.getData().color;
}

DirectX::XMFLOAT3 SpotLight::getPosition()
{
	return buffer.getData().position;
}

void SpotLight::setbuffer(int startSlot, int numBuffer, ID3D11DeviceContext* immediateContext)
{
	buffer.applyData();
	immediateContext->CSSetConstantBuffers(startSlot, numBuffer, buffer.getReferenceOf());
}


