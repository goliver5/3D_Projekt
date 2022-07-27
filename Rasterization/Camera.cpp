#include "Camera.h"
#include "directxmath.h"

using namespace DirectX;

Camera::Camera()
{
	eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	focusPosition = DirectX::XMVectorSet(0.0f, 0.f, 1.f, 0.0f);
	upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	rot.x = 0.0f;
	rot.y = 0.0f;
	rotVector = XMLoadFloat3(&rot);
	rotationForMatrix = XMMatrixRotationRollPitchYawFromVector(rotVectorFor);
	rotationMX = DirectX::XMMatrixRotationRollPitchYawFromVector(rotVectorFor);



	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, DEFAULT_UP);
	//projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1.7f, 1.0f, 1011111.0f);
	projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1024/576, 0.1f, 1000.0f);

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
	// ändra rotation så att kameran inte buggar på första move NÄR rotationMX är 0 kan fixa senare om jag initierar den i konstruktorn
	//this->setRotation(0.01f, 0.01f, immediateContext);

	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);

	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upVector);
	viewProjectionMatrix = view * projection;
	viewProjectionMatrix = DirectX::XMMatrixTranspose(viewProjectionMatrix);
	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
	VPcBuffer->applyData();

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
	if (GetAsyncKeyState('D'))
	{
		rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
		eyePosition += rightVec * 0.02f;
		focusPosition += rightVec * 0.02f;
	}
	if (GetAsyncKeyState('S'))
	{
		forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForMatrix);
		eyePosition -= forwardVec * 0.02f;
		focusPosition -= forwardVec * 0.02f;
	}
	if (GetAsyncKeyState('W'))
	{
		forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForMatrix);
		eyePosition += forwardVec * 0.02f;
		focusPosition += forwardVec * 0.02f;
	}

	if (GetAsyncKeyState('K'))//Ner rotation
	{
		this->changeRotation(0.02, 0.0f, immediateContext);
	}
	if (GetAsyncKeyState('I'))//Up rotation
	{
		this->changeRotation(-0.02, 0.0f, immediateContext);
	}
	if (GetAsyncKeyState('J'))//Vänster rotation
	{
		this->changeRotation(0.0f, -0.02f, immediateContext);
	}
	if (GetAsyncKeyState('L'))//Höger rotation
	{
		this->changeRotation(0.0f, 0.02f, immediateContext);
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

void Camera::changeRotation(float x, float y, ID3D11DeviceContext* immediateContext)
{
	const float max = 1.5f;
	const float min = -1.5f;

	rot.x += x;
	rot.y += y;
	rotVector = XMLoadFloat3(&rot);

	rotFor.y = y;
	rotVectorFor = XMLoadFloat3(&rotFor);

	rotationForMatrix = XMMatrixRotationRollPitchYawFromVector(rotVectorFor);
	rotationMX = XMMatrixRotationRollPitchYawFromVector(rotVector);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
	focusPosition = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX) + eyePosition;
}

void Camera::setLightTemp()
{
	eyePosition = DirectX::XMVectorSet(0.0f, 5.0f, -3.0f, 0.0f);
	focusPosition = DirectX::XMVectorSet(0.0f, 0.f, 1.f, 0.0f);
	upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, DEFAULT_UP);
	//projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1.7f, 1.0f, 1011111.0f);
	projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1024 / 576, 0.1f, 1000.0f);

	DirectX::XMMATRIX viewProjection = view * projection;
	viewProjection = DirectX::XMMatrixTranspose(viewProjection);
	this->viewProjectionMatrix = viewProjection;

	view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upVector);
	viewProjectionMatrix = view * projection;
	viewProjectionMatrix = DirectX::XMMatrixTranspose(viewProjectionMatrix);
	DirectX::XMStoreFloat4x4(&VPcBuffer->getData().VPMatrix, this->viewProjectionMatrix);
	VPcBuffer->applyData();
}

void Camera::setPosition(float x, float y, float z, ID3D11DeviceContext* immediateContext)
{
	XMFLOAT3 newPos;
	newPos.x = x;
	newPos.y = y;
	newPos.z = z;
	eyePosition = XMVectorSet(newPos.x, newPos.y, newPos.z, 0.0f);
	focusPosition = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX) + eyePosition;
}

cameraForwardUpvector Camera::particleTempCamera(ID3D11DeviceContext* immediateContext)
{
	cameraForwardUpvector value;

	forwardVec = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);
	upVector = DirectX::XMVector3TransformCoord(DEFAULT_UP, rotationMX);

	DirectX::XMStoreFloat3(&value.upVector, upVector);
	DirectX::XMStoreFloat3(&value.forwardVector, forwardVec);

	return value;
}

void Camera::setviewProjectionLightVertexShader(int startSlot, int numBuffers, ID3D11DeviceContext* immediateContext)
{
	immediateContext->VSSetConstantBuffers(startSlot, numBuffers, VPcBuffer->getReferenceOf());
}

DirectX::XMVECTOR Camera::getcameraPosition()
{
	return position;
}
