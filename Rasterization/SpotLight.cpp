#include "SpotLight.h"
SpotLight::SpotLight()
{
}
bool SpotLight::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool value = buffer.Initialize(device, deviceContext);

	buffer.getData().position = DirectX::XMFLOAT3(-10.0f, 10.0f, 5.0f);
	buffer.getData().color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer.getData().direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	buffer.getData().attenuation = DirectX::XMFLOAT3(0.7f, 0.2f, 0.0f);
	buffer.getData().cone = 5.0f;
	buffer.getData().range = 5.0f;

	return value;
}

void SpotLight::setPosition(float x, float y, float z)
{
	
}

void SpotLight::setbuffer(int startSlot, int numBuffer, ID3D11DeviceContext* immediateContext)
{
	buffer.applyData();
	immediateContext->PSSetConstantBuffers(startSlot, numBuffer, buffer.getReferenceOf());
}


