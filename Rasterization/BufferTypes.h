#pragma once
#include <DirectXMath.h>

struct matrixes //use for storage
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
};

struct WVPMatrix // use for upload cBuffer
{
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

struct VPMatrix
{
	DirectX::XMFLOAT4X4 VPMatrix;
};

struct WMatrix
{
	DirectX::XMFLOAT4X4 world;
};

struct position
{
	DirectX::XMFLOAT3 position;
	float padding;
};