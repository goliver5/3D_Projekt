#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{	
}

bool DirectionalLight::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool value = buffer.Initialize(device, deviceContext);

	buffer.getData().direction = DirectX::XMFLOAT3(-0.0f, -.5f, -.5f);
	buffer.getData().color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer.getData().padd = 0.0f;
	buffer.getData().padd2 = 0.0f;

	buffer.applyData();
	return value;
}

void DirectionalLight::setbuffer(int startSlot, int numBuffer, ID3D11DeviceContext* immediateContext)
{
	buffer.applyData();
	immediateContext->CSSetConstantBuffers(startSlot, numBuffer, buffer.getReferenceOf());
}
