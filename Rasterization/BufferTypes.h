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

struct particleInformation // Compute shader use for delta time and number of particles per thread
{
	float time;
	float pPerThread;
	float padding[2];
};

struct cameraForwardUpvector
{
	DirectX::XMFLOAT3 upVector;
	float SIZE = 0.12f;
	DirectX::XMFLOAT3 forwardVector;
	float padding2 = 0;
};

struct lightBuf
{
	DirectX::XMFLOAT3 lightPos;
	float ambienceStrength;
	DirectX::XMFLOAT3 cameraPos;
	float padd;
};

struct cameraPosition
{
	DirectX::XMFLOAT3 cameraPos;
	float padding;
};

struct dirLight
{

};

struct spotLight
{
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 direction;
	float cone;
	DirectX::XMFLOAT3 color;
	float pad;
	DirectX::XMFLOAT3 attenuation;
	float pad2;
};
