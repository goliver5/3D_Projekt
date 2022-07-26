#include "LightCBuffer.h"
#include <iostream>

LightCBuffer::LightCBuffer()
{
	DirectX::XMStoreFloat3(&lBuf.cameraPos, DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f));
	lBuf.ambienceStrength = 0.9f;
	DirectX::XMStoreFloat3(&lBuf.lightPos, DirectX::XMVectorSet(2.0f, 0.0f, -2.0f, 0.0f));
	lBuf.padd = 1.0f;
}

void LightCBuffer::updateLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext)
{
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT hr = immediateContext->Map(lightConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
	memcpy(mappedResource.pData, &lBuf, sizeof(lightBuf));
	immediateContext->Unmap(lightConstantBuffer, 0);
}

bool LightCBuffer::setLightCBuffer(ID3D11Device*& device, ID3D11Buffer*& lightConstantBuffer, ID3D11DeviceContext*& immediateContext)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.StructureByteStride = 0;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = sizeof(lightBuf);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &lBuf;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &lightConstantBuffer);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create constant light Buffer!\n";
		return false;
	}
	//immediateContext->VSSetConstantBuffers(1, 1, &lightConstantBuffer);
	return true;
}

void LightCBuffer::setPosition(float x, float y, float z, ID3D11DeviceContext* immediateContext, ID3D11Buffer*& lightConstantBuffer)
{
	DirectX::XMFLOAT3 tempPos(x, y, z);
	
	lBuf.cameraPos = tempPos;
}
