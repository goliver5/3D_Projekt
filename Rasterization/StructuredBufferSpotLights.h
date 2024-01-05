#pragma once
#include "StructureBuffer.h"
#include "Camera.h"
#include <d3d11.h>

class StructuredBufferSpotLights
{
private:
	StructuredBuffer<spotLightData> buffer;
public:
	StructuredBufferSpotLights();
	bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void setColor(float r, float g, float b, const int index);
	void setPosition(float x, float y, float z, const int index);
	void setDirection(float x, float y, float z, const int index);

	DirectX::XMFLOAT3 getColor(const int index);
	DirectX::XMFLOAT3 getPosition(const int index);
	void setbufferCS(const int toRegister, ID3D11DeviceContext* immediateContext);
	void addSpotLight(ID3D11Device* device);
};