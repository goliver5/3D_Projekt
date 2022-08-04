#include "SceneObject.h"

HRESULT SceneObject::createVertexBuffer(ID3D11Device* device)
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(VertexData) * vertexForIndex.size();
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = vertexForIndex.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
    return hr;
}

bool SceneObject::createIndexBuffer(ID3D11Device* device)
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(int) * indices.size();
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = indices.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &indexBuffer);
    if (FAILED(hr)) return false;

    return true;
}

SceneObject::SceneObject(ID3D11Device *device, ID3D11DeviceContext* immediateContext, ID3D11ShaderResourceView*& textureSRVs, std::string fileName)
{
    textureSRV = textureSRVs;

    constantBuffer.Initialize(device, immediateContext);
    world = DirectX::XMMatrixIdentity();
    DirectX::XMStoreFloat4x4(&constantBuffer.getData().world, world);

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uv;

	ParseOBJFile(vertices, normals, uv, vertexForIndex, vertexSubMeshCounter, indices, fileName);
	
    UINT verticeplace = 0;
    UINT uvPlace = 0;

    //loopar igenom för hur många vertexData som behövs
    //for (int i = 0; i < vertices.size()/3; i++)
    //{
    //    std::vector<float> tempVert;
    //    std::vector<float> tempNorm;
    //    std::vector<float> tempUv;

    //    for (int i = 0; i < 3; i++)
    //    {
    //        tempVert.push_back(vertices[verticeplace]);
    //        tempNorm.push_back(normals[verticeplace]);
    //        verticeplace++;
    //    }

    //    for (int i = 0; i < 2; i++)
    //    {
    //        tempUv.push_back(uv[uvPlace]);
    //        uvPlace++;
    //    }
    //    vertexData.push_back(VertexData(tempVert, tempNorm, tempUv));
    //    tempVert.clear();
    //    tempNorm.clear();
    //    tempUv.clear();
    //}
    createVertexBuffer(device);
    createIndexBuffer(device);
    setGroundPos();
}

void SceneObject::setGroundPos()
{
    world = DirectX::XMLoadFloat4x4(&constantBuffer.getData().world);
    world = DirectX::XMMatrixTranspose(world);
    world *= DirectX::XMMatrixTranslation(0.0f, -3.0f, 0.0f);

    //world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);
    world = DirectX::XMMatrixTranspose(world);
}

void SceneObject::setPosition(float x, float y, float z)
{
    world = DirectX::XMMatrixIdentity();
    world *= DirectX::XMMatrixTranslation(x, y, z);
    world = DirectX::XMMatrixTranspose(world);
    XMStoreFloat4x4(&constantBuffer.getData().world, world);
    constantBuffer.applyData();
}

void SceneObject::rotateObject(float x, float y, float z)
{
    //temp scalar och roterar objektet för golvet
    world = DirectX::XMLoadFloat4x4(&constantBuffer.getData().world);
    world = DirectX::XMMatrixTranspose(world);
    //world = DirectX::XMMatrixTranslation(10.0f, 0.0f, 10.0f);
    //world = world * DirectX::XMMatrixRotationX(x);
    world = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(10.0f, 12.0f, 12.0f), world);
    world = DirectX::XMMatrixTranspose(world);
}

void SceneObject::tempUpdate()
{
    world = DirectX::XMLoadFloat4x4(&constantBuffer.getData().world);
    world = DirectX::XMMatrixTranspose(world);
    world *= DirectX::XMMatrixTranslation(0.0f, .0f, -1.0f);
    static float rotationAmount = 0.25f / 360.f * DirectX::XM_2PI;

    world *= DirectX::XMMatrixRotationY(rotationAmount);

    world *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);
    world = DirectX::XMMatrixTranspose(world);
}

void SceneObject::noMemoryLeak()
{
    //textureSRV->Release();
    vertexBuffer->Release();
    indexBuffer->Release();
}

void SceneObject::updateBuffer()
{
    XMStoreFloat4x4(&constantBuffer.getData().world, world);
    constantBuffer.applyData();
}

void SceneObject::draw(ID3D11DeviceContext*& immediateContext)
{
    XMStoreFloat4x4(&constantBuffer.getData().world, world);
    constantBuffer.applyData();
    immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.getReferenceOf());
    immediateContext->PSSetShaderResources(0, 1, &textureSRV);

    UINT stride = sizeof(VertexData);
    UINT offset = 0;
    int size = vertexData.size();

    //immediateContext->Draw(size, 0);
    int counter = 0;
    immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    for (int i = 0; i < vertexSubMeshCounter.size(); i++)
    {
        //skicka in submeshes i pixelshadern
        immediateContext->DrawIndexed(vertexSubMeshCounter[i], counter, 0);
        counter += vertexSubMeshCounter[i];
    }

}

void SceneObject::drawCubeMap(ID3D11DeviceContext*& immediateContext)
{
    XMStoreFloat4x4(&constantBuffer.getData().world, world);
    constantBuffer.applyData();
    immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.getReferenceOf());
    immediateContext->PSSetShaderResources(0, 1, &textureSRV);

    UINT stride = sizeof(VertexData);
    UINT offset = 0;
    int size = vertexData.size();

    //immediateContext->Draw(size, 0);
    int counter = 0;
   // immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    immediateContext->Draw(size, 0);
}
