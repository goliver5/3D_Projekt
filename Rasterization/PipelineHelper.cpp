#include "PipelineHelper.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode, ID3D11ComputeShader*& cShader)
{
    std::string shaderData;
    std::ifstream reader;
    reader.open("../x64/Debug/VertexShader.cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        std::cerr << "Could not open vertex shader file!" << std::endl;
        return false;
    }

    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);

    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

    if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
    {
        std::cerr << "Failed to create vertex shader!" << std::endl;
        return false;
    }

    vShaderByteCode = shaderData;
    shaderData.clear();
    reader.close();
    reader.open("../x64/Debug/PixelShader.cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        std::cerr << "Could not open PS file!" << std::endl;
        return false;
    }
    shaderData.clear();
    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);

    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

    if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
    {
        std::cerr << "Failed to create pixel shader!" << std::endl;
        return false;
    }



    shaderData.clear();
    reader.close();
    reader.open("../x64/Debug/ComputeShader.cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        std::cerr << "Could not open CS file!" << std::endl;
        return false;
    }

    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);

    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
    HRESULT hr;
    hr = device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &cShader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create compute shader!" << std::endl;
        return false;
    }

    return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
    D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    HRESULT hr = device->CreateInputLayout(inputDesc, _countof(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

    return !FAILED(hr);
}

bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer)
{
   /* SimpleVertex triangle[3] =
    {
        { {-0.5f, 0.5f, 0.0f}, {0, 0, 1} },
        { {0.5f, -0.5f, 0.0f}, {0, 1, 0} },
        { {-0.5f, -0.5f, 0.0f}, {1, 0, 0} }
    };*/

    SimpleVertex triangle[6] =
    {
        { {-0.5f, 0.5f, 0.0f}, {0, 0, -1}, {0, 0} },
        { {0.5f, -0.5f, 0.0f}, {0, 0, -1}, {1, 1} },
        { {-0.5f, -0.5f, 0.0f}, {0, 0, -1}, {0, 1} },

        { {-0.5f, 0.5f, 0.0f}, {0, 0, -1}, {0, 0} },
        { {0.5f, 0.5f, 0.0f}, {0, 0, -1}, {1, 0} },
        { {0.5f, -0.5f, 0.0f}, {0, 0, -1}, {1, 1} }
    };
    
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(triangle);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = triangle;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
    return !FAILED(hr);
}

bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV)
{
   /* const int width = 472;
    const int height = 472;
    std::vector<unsigned char> textureData;
    textureData.resize(width * height * 4);

    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            unsigned char r = w < width / 3 ? 255 : 0;
            unsigned char g = w >= width / 3 && w <= width / 1.5 ? 255 : 0;
            unsigned char b = w > width / 1.5 ? 255 : 0;
            unsigned int pos0 = w * 4 + width * 4 * h;
            textureData[pos0 + 0] = r;
            textureData[pos0 + 1] = g;
            textureData[pos0 + 2] = b;
            textureData[pos0 + 3] = 255;
        }
    }*/

    int width = 472;
    int height = 472;
    int comp = 4;
    int channels = 4;
    //std::vector<unsigned char> textureData;
    //textureData.resize(width * height * 4);
    unsigned char* textureData = stbi_load("beel.png", &width, &height, &comp, channels);

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &textureData[0];
    data.SysMemPitch = width * 4;
    data.SysMemSlicePitch = 0;

    if (FAILED(device->CreateTexture2D(&desc, &data, &texture)))
    {
        std::cerr << "Failed to create texture!" << std::endl;
        return false;
    }
    HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &textureSRV);
    
    stbi_image_free(textureData);
    return !FAILED(hr);
}

bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& sampler)
{
    D3D11_SAMPLER_DESC desc;
    desc.Filter = D3D11_FILTER_ANISOTROPIC;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.MipLODBias = 0;
    desc.MaxAnisotropy = 16;
    desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState(&desc, &sampler);
    return !FAILED(hr);
}

bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
    ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV, ID3D11SamplerState*& sampler, ID3D11ComputeShader*& cShader)
{
    std::string vShaderByteCode;
    if (!LoadShaders(device, vShader, pShader, vShaderByteCode, cShader))
    {
        std::cerr << "Error loading shaders!" << std::endl;
        return false;
    }

    if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
    {
        std::cerr << "Error creating input layout!" << std::endl;
        return false;
    }

    if (!CreateVertexBuffer(device, vertexBuffer))
    {
        std::cerr << "Error creating vertex buffer!" << std::endl;
        return false;
    }

    if (!CreateTexture(device, texture, textureSRV))
    {
        std::cerr << "Error creating texture!" << std::endl;
        return false;
    }

    if (!CreateSamplerState(device, sampler))
    {
        std::cerr << "Error creating sampler state!" << std::endl;
        return false;
    }

    return true;
}
