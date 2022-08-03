#include "CubeMapping.h"

bool CubeMapping::setupPSShader(ID3D11Device* device)
{
    std::string shaderData;
    std::ifstream reader;

    shaderData.clear();
    reader.close();
    reader.open("../x64/Debug/SamplepixelShader.cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        return false;
    }

    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);

    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

    HRESULT hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &SamplepixelShader);
    if (FAILED(hr))
    {
        return false;
    }
    shaderData.clear();
    reader.close();


    shaderData.clear();
    reader.close();
    reader.open("../x64/Debug/drawCubePixelShader.cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        return false;
    }

    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);

    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

    hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &drawCubePixelShader);
    if (FAILED(hr))
    {
        return false;
    }
    shaderData.clear();
    reader.close();

	return true;
}

bool CubeMapping::setupDepthStencil(ID3D11Device* device)
{
    ID3D11Texture2D* texture;

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = 1024;
    textureDesc.Height = 1024;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.MiscFlags = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MipLevels = 1;

    HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
    if (FAILED(hr))return false;

    hr = device->CreateDepthStencilView(texture, nullptr, &depthStencilView);
    if (FAILED(hr))return false;

    texture->Release();
    return true;
}

bool CubeMapping::setupRTVandSRV(ID3D11Device* device)
{
    ID3D11Texture2D* texture;

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = 1024;
    textureDesc.Height = 1024;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MipLevels = 1;

    HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
    if (FAILED(hr))return false;

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvDesc.Texture2DArray.ArraySize = 1;
    rtvDesc.Texture2DArray.MipSlice = 0;

    for (int i = 0; i < 6; i++)
    {
        rtvDesc.Texture2DArray.FirstArraySlice = (UINT)i;
        hr = device->CreateRenderTargetView(texture, &rtvDesc, &cubeRtv[i]);
        if (FAILED(hr))return false;
    }

    hr = device->CreateShaderResourceView(texture, nullptr, &cubeSrv);

    texture->Release();
	return true;
}

bool CubeMapping::setupSampler(ID3D11Device* device)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MipLODBias = 0;

    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 0.0f;

    HRESULT hr = device->CreateSamplerState(&samplerDesc, &cubeSampler);
    if (FAILED(hr)) return false;
    return true;
}

CubeMapping::CubeMapping(SceneObject &obj, UINT WIDTH, UINT HEIGHT)
    :WIDTH(WIDTH),HEIGHT(HEIGHT)
{
    this->cubeObject = &obj;
    cubeSrv = nullptr;
    SamplepixelShader = nullptr;
    drawCubePixelShader = nullptr;
    depthStencilView = nullptr;
    cubeSampler = nullptr;
    for (int i = 0; i < 6; i++)
    {
        cubeRtv[i] = nullptr;
    }
}

CubeMapping::~CubeMapping()
{
    for (int i = 0; i < 6; i++)
    {
        cubeRtv[i]->Release();
    }
    cubeSrv->Release();
    drawCubePixelShader->Release();
    SamplepixelShader->Release();
    depthStencilView->Release();
    cubeObject->noMemoryLeak();
    cubeSampler->Release();
}

bool CubeMapping::initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	if (!setupPSShader(device)) return false;
	if (!setupDepthStencil(device))return false;
	if (!setupRTVandSRV(device))return false;
    if (!setupSampler(device))return false;

    camera.initializeCamera(device, immediateContext, VPBuf);
    cameraPositionBuffer.Initialize(device, immediateContext);

    camera.setPosition(0.0f, 0.0f, 2.0f);
    cubeObject->setPosition(0.0f, 0.0f, 2.0f);

    float tempnintendo = DirectX::XM_PI / 2;

    cameraRotation.push_back({ 0.0f, tempnintendo, 0.0f });
    cameraRotation.push_back({ 0.0f, -tempnintendo, 0.0f });
    cameraRotation.push_back({ -tempnintendo, 0.0f, 0.0f });
    cameraRotation.push_back({ tempnintendo, 0.0f, 0.0f });
    cameraRotation.push_back({ 0.0f, 0.0f, 0.0f });
    cameraRotation.push_back({ 0.0f, -tempnintendo* 2, 0.0f });
    //göra för resten

	return true;
}

void CubeMapping::firstPass(ID3D11DeviceContext* immediateContext, std::vector<SceneObject>& sceneObjects)
{
    //roterar kameran i varje draw call ritar till rtv, clear mellan varje draw call
    //6 draw calls
    //immediateContext->VSSetShader()
    immediateContext->PSSetShader(SamplepixelShader, nullptr, 0);
    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

    for (int i = 0; i < 6; i++)
    {
        camera.setRotation(cameraRotation[i].x, cameraRotation[i].y, cameraRotation[i].z);
        camera.setviewProjectionLightVertexShader(1, 1, immediateContext);
        immediateContext->ClearRenderTargetView(cubeRtv[i], clearColor);
        immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
        immediateContext->OMSetRenderTargets(1, &cubeRtv[i], depthStencilView);

        for (int j = 0; j < sceneObjects.size(); j++)
        {
            sceneObjects[j].draw(immediateContext);
        }
    }

    ID3D11RenderTargetView* nullRtv2 = nullptr;
    immediateContext->OMSetRenderTargets(1, &nullRtv2, nullptr);
   /* ID3D11ShaderResourceView* nullrsv = nullptr;
    immediateContext->PSSetShaderResources(0, 1, &nullrsv);
    immediateContext->PSSetShaderResources(1, 1, &nullrsv);*/

}

void CubeMapping::drawCube(ID3D11DeviceContext* immediateContext)
{

    immediateContext->PSSetShader(drawCubePixelShader, nullptr, 0);
    immediateContext->PSSetShaderResources(1, 1, &cubeSrv);
    //immediateContext->PSSetSamplers(1, 1, SamplepixelShader);

    //DirectX::XMStoreFloat3(&cameraPositionBuffer.getData().cameraPos, camera.getcameraPosition());
    //cameraPositionBuffer.getData().padding = 1.0f;

    // immediateContext->PSSetConstantBuffers(0, 1, cameraPositionBuffer.getReferenceOf());
    //cameraPositionBuffer.applyData();
    
    immediateContext->PSSetSamplers(0, 1, &cubeSampler);
    //immediateContext->PSSetShaderResources(0, 1, &cubeSrv);
    cubeObject->draw(immediateContext);
    
    //ID3D11RenderTargetView* nullrv = nullptr;
    //immediateContext->OMSetRenderTargets(1, &nullrv, nullptr);
    //immediateContext->PSSetShader(nullptr, nullptr, 0);

}


