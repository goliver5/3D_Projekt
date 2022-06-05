#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

struct lightBuf
{
	DirectX::XMFLOAT3 lightPos;
	float ambienceStrength;
	DirectX::XMFLOAT3 cameraPos;
	float padd;
};

class LightCBuffer
{
private:
	lightBuf lBuf;

public:
	LightCBuffer();

	void updateLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext);
	bool setLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext);
};