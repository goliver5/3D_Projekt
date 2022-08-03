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
	DirectX::XMMATRIX rotationForMatrix;

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	
	DirectX::XMFLOAT3 rot;
	DirectX::XMFLOAT3 pos;

	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR focusPosition;
	DirectX::XMVECTOR upDirection;

	DirectX::XMFLOAT3 rotFor;
	DirectX::XMVECTOR rotVector;
	DirectX::XMVECTOR rotVectorFor = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR forwardVec = DEFAULT_FORWARD;
	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR rightVec = DEFAULT_RIGHT;

	//VPMatrix vpMatrix;
	ConstantBufferNew<VPMatrix> *VPcBuffer;
	ConstantBufferNew<cameraPosition> cameraPositionBuffer;
public:
	Camera();

	bool initializeCamera(ID3D11Device* device, ID3D11DeviceContext*& immediateContext, ConstantBufferNew<VPMatrix>& cBuffer);
	void update(ID3D11DeviceContext* immediateContext);
	void ShadowUpdateDebug(ID3D11DeviceContext* immediateContext);

	void setVSBuffer(ID3D11DeviceContext*& immediateContext);
	void setGSViewProjectionBuffer(ID3D11DeviceContext*& immediateContext);
	void changeRotation(float x, float y, ID3D11DeviceContext* immediateContext);

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);

	void setLightTemp(ID3D11DeviceContext*& immediateContext);
	void setPosition(float x, float y, float z, ID3D11DeviceContext* immediateContext);
	cameraForwardUpvector particleTempCamera(ID3D11DeviceContext* immediateContext);

	void setviewProjectionLightVertexShader(int startSlot, int numBuffers, ID3D11DeviceContext *immediateContext);
	void setPSCameraPosition(ID3D11DeviceContext* immediateContext);

	DirectX::XMVECTOR getcameraPosition();
};