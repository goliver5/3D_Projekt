#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

#include"BufferTypes.h"

class LightCBuffer
{
private:
	lightBuf lBuf;

public:
	LightCBuffer();

	void updateLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext);
	bool setLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext);
	void setPosition(float x, float y, float z, ID3D11DeviceContext* immediateContext, ID3D11Buffer*& lightConstantBuffer);
};