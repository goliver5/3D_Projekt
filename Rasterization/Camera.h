#pragma once
//#include "BufferTypes.h"
#include "ConstantBufferNew.h"

class Camera
{
private:
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX viewProjectionMatrix;

	DirectX::XMVECTOR position;
	DirectX::XMMATRIX rotationMX;

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	
	DirectX::XMFLOAT3 rot;
	DirectX::XMFLOAT3 pos;

	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR focusPosition;
	DirectX::XMVECTOR upDirection;

	DirectX::XMVECTOR forwardVec = DEFAULT_FORWARD;
	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR rightVec = DEFAULT_RIGHT;
	VPMatrix vpMatrix;
	ConstantBufferNew<VPMatrix> *VPcBuffer;
public:
	Camera();
	bool initializeCamera(ID3D11Device* device, ID3D11DeviceContext*& immediateContext, ConstantBufferNew<VPMatrix>& cBuffer);
	void update();
	void setVSBuffer(ID3D11DeviceContext*& immediateContext);
};