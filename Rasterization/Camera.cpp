#include "Camera.h"
#include "directxmath.h"

using namespace DirectX;

Camera::Camera()
{
	eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
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
	this->rotVector = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

bool Camera::initializeCamera(ID3D11Device* device, ID3D11DeviceContext*& immediateContext, ConstantBufferNew<VPMatrix>& cBuffer)
{
	cBuffer.Initialize(device, immediateContext);
	this->VPcBuffer = &cBuffer;

	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
	return true;
}

void Camera::update(ID3D11DeviceContext* immediateContext)
{
	if (GetAsyncKeyState('A'))
	{
		rightVec = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
		eyePosition -= rightVec * 0.02f;
		focusPosition -= rightVec * 0.02f;
	}
	else if (GetAsyncKeyState('D'))
	{
		rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
		eyePosition += rightVec * 0.02f;
		focusPosition += rightVec * 0.02f;
	}
	else if (GetAsyncKeyState('S'))
	{
		forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForMatrix);
		eyePosition -= forwardVec * 0.02f;
		focusPosition -= forwardVec * 0.02f;
	}
	else if (GetAsyncKeyState('W'))
	{
		forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForMatrix);
		eyePosition += forwardVec * 0.02f;
		focusPosition += forwardVec * 0.02f;
	}
	else if (GetAsyncKeyState('K'))//Ner rotation
	{
		this->setRotation(0.02, 0.0f, immediateContext);
	}
	else if (GetAsyncKeyState('I'))//Up rotation
	{
		this->setRotation(-0.02, 0.0f, immediateContext);
	}
	else if (GetAsyncKeyState('J'))//Vänster rotation
	{
		this->setRotation(0.0f, -0.02f, immediateContext);
	}
	else if (GetAsyncKeyState('L'))//Höger rotation
	{
		this->setRotation(0.0f, 0.02f, immediateContext);
	}

	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upVector);
	viewProjectionMatrix = view * projection;
	viewProjectionMatrix = DirectX::XMMatrixTranspose(viewProjectionMatrix);
	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
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
	immediateContext->GSSetConstantBuffers(0, 1, VPcBuffer->getReferenceOf());
}

void Camera::setRotation(float x, float y, ID3D11DeviceContext* immediateContext)
{
	const float max = 1.5f;
	const float min = -1.5f;

	rot.x += x;
	rot.y += y;
	rotVector = XMLoadFloat3(&rot);

	rot.y += y;
	rotVectorFor = XMLoadFloat3(&rotFor);

	//låser rotationen så man inte roterar
	//if (rot.x > max)
	//{
	//	rot.x = max;
	//}
	//else if (rot.x < min)
	//{
	//	rot.x = min;
	//}

	rotationForMatrix = XMMatrixRotationRollPitchYawFromVector(rotVectorFor);
	rotationMX = XMMatrixRotationRollPitchYawFromVector(rotVector);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
	focusPosition = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX) + eyePosition;
}

DirectX::XMVECTOR Camera::getcameraPosition()
{
	return position;
}
