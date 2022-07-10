#include "Camera.h"
#include "directxmath.h"

using namespace DirectX;

Camera::Camera()
{
	eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	focusPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);
	upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, DEFAULT_UP);
	projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1.7f, 1.0f, 10.0f);
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(100), 1.7f, 0.1f, 10.0f);

	DirectX::XMMATRIX viewProjection = view * projection;
	viewProjection = DirectX::XMMatrixTranspose(viewProjection);
	this->viewProjectionMatrix = viewProjection;


	/*this->forwardVec = DEFAULT_FORWARD;
	this->upVector = DEFAULT_UP;
	this->rightVec = DEFAULT_RIGHT;*/
	
	position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

bool Camera::initializeCamera(ID3D11Device* device, ID3D11DeviceContext*& immediateContext, ConstantBufferNew<VPMatrix>& cBuffer)
{
	cBuffer.Initialize(device, immediateContext);
	this->VPcBuffer = &cBuffer;

	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
	return true;
}

void Camera::update()
{
	if (GetAsyncKeyState('A'))
	{
		
		eyePosition -= rightVec * 0.02f;
		focusPosition -= rightVec * 0.02f;
	}
	else if (GetAsyncKeyState('D'))
	{
		eyePosition += rightVec * 0.02f;
		focusPosition += rightVec * 0.02f;
	}
	else if (GetAsyncKeyState('S'))
	{
		eyePosition -= forwardVec * 0.02f;
		focusPosition -= forwardVec * 0.02f;
	}
	else if (GetAsyncKeyState('W'))
	{
		eyePosition += forwardVec * 0.02f;
		focusPosition += forwardVec * 0.02f;

	}
	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upVector);
	viewProjectionMatrix = view * projection;
	viewProjectionMatrix = DirectX::XMMatrixTranspose(viewProjectionMatrix);
	XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
	VPcBuffer->applyData();

	//kanske behövs kolla senare
	//DirectX::XMStoreFloat3(&pos, eyePosition);

}

void Camera::setVSBuffer(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->VSSetConstantBuffers(1, 1, this->VPcBuffer->getReferenceOf());
}

void Camera::setGSViewProjectionBuffer(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->GSSetConstantBuffers(0, 1, this->VPcBuffer->getReferenceOf());
}

DirectX::XMVECTOR Camera::getcameraPosition()
{
	return position;
}
