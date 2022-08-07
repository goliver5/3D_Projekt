#pragma once
#include "ConstantBufferNew.h"

class DirectionalLight
{
private:
	ConstantBufferNew<dirLight> buffer;
public:
	DirectionalLight();
	bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void setColor(float x, float y, float z);
	DirectX::XMFLOAT3 getColor();
	void setbuffer(int startSlot, int numBuffer, ID3D11DeviceContext* immediateContext);
};