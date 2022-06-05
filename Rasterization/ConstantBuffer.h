#pragma once
#include "BufferTypes.h"
#include <d3d11.h>

class ConstantBuffer
{

	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	//DirectX::XMMATRIX world;
	//DirectX::XMMATRIX view;
	//DirectX::XMMATRIX projection;
public:
	ConstantBuffer();
	//void SetupConstantBuffer(ID3D11Device*& device)
	void updateConstantBuffer(ID3D11Device*& device, ID3D11Buffer*& constantBuffer, ID3D11DeviceContext*& immediateContext);
	bool setConstantBuffer(ID3D11Device*& device, ID3D11Buffer*& constantBuffer, ID3D11DeviceContext*& immediateContext);
};