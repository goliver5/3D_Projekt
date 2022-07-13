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

struct VPMatrix // view projection matrix
{
	DirectX::XMFLOAT4X4 VPMatrix;
};

struct WMatrix // world matrix
{
	DirectX::XMFLOAT4X4 world;
};

struct position // particle positions
{
	DirectX::XMFLOAT3 position;
	float padding;
};

struct particleInformation
{
	float time;
	float pPerThread;
	float padding[2];
};