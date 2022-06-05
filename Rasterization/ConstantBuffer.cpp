#include "ConstantBuffer.h"
#include <iostream>
#include <chrono>

ConstantBuffer::ConstantBuffer()
{
	world = DirectX::XMMatrixIdentity();
	//world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f);//xmmatrixidentity
	//this->world *= DirectX::XMMatrixRotationY(0.0001f);
	//world *= DirectX::XMMatrixInverse(DirectX::XMMatrixDeterminant(world), world);
	world = DirectX::XMMatrixTranspose(this->world);
	

	DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	DirectX::XMVECTOR focusPosition = DirectX::XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f);
	DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1.7f, 1.0f, 10.0f);
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(100), 1.7f, 0.1f, 10.0f);

	viewProjection = view * projection;
	viewProjection = DirectX::XMMatrixTranspose(viewProjection);
	viewProjection = viewProjection;
	//DirectX::

	//DirectX::XMFLOAT4X4 worldMatrix;
	XMStoreFloat4x4(&worldMatrix, world);
	//DirectX::XMFLOAT4X4 viewAndProject;
	XMStoreFloat4x4(&projectionMatrix, viewProjection);
}

void ConstantBuffer::updateConstantBuffer(ID3D11Device*& device, ID3D11Buffer*& constantBuffer, ID3D11DeviceContext*& immediateContext )
{
	this->world = DirectX::XMLoadFloat4x4(&worldMatrix);
	this->world = DirectX::XMMatrixTranspose(this->world);
	world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, -1.0f);
	static float rotationAmount = 0.25f / 360.f * DirectX::XM_2PI;

	this->world *= DirectX::XMMatrixRotationY(rotationAmount);

	world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);
	//world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.1f);
	
	//world = DirectX::XMMatrixMultiply(this->world, this->viewProjection);
	this->world = DirectX::XMMatrixTranspose(this->world);
	XMStoreFloat4x4(&worldMatrix, world);
	XMStoreFloat4x4(&projectionMatrix, viewProjection);

	WVPMatrix test2 = {
		{worldMatrix},
		{projectionMatrix
			} };
			
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT hr = immediateContext->Map(constantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
	memcpy(mappedResource.pData, &test2, sizeof(test2));
	immediateContext->Unmap(constantBuffer, 0);
}

bool ConstantBuffer::setConstantBuffer(ID3D11Device*& device, ID3D11Buffer*& constantBuffer, ID3D11DeviceContext*& immediateContext)
{

	WVPMatrix test2 = {
	{worldMatrix},
	{projectionMatrix
		} };
		
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.StructureByteStride = 0;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = sizeof(test2);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &test2;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create constant Buffer!\n";
		return false;
	}

	//immediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	return true;
	
}
