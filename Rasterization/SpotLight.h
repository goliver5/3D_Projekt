#pragma once
#include"ConstantBufferNew.h"
#include "Camera.h"
#include<d3d11.h>

class SpotLight
{
private:
	ConstantBufferNew<spotLightData> buffer;
public:
	SpotLight();
	bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	void setColor(float r, float g, float b);
	void setPosition(float x, float y, float z);
	void setDirection(float x, float y, float z);

	DirectX::XMFLOAT3 getColor();
	DirectX::XMFLOAT3 getPosition();
	void setbuffer(int startSlot, int numBuffer, ID3D11DeviceContext* immediateContext);
};