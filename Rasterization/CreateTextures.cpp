#include "CreateTextures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool createTextures(ID3D11Device* device, ID3D11ShaderResourceView*& textureSRV, std::string fileName)
{
	ID3D11Texture2D* texture;

	int width = 472;
	int height = 472;
	int comp = 4;
	int channels = 4;
	//std::vector<unsigned char> textureData;
	//textureData.resize(width * height * 4);
	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &comp, channels);

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

	HRESULT hr = device->CreateTexture2D(&desc, &data, &texture);

	if (FAILED(hr))
	{
		//std::cerr << "Failed to create texture!" << std::endl;
		return false;
	}
	hr = device->CreateShaderResourceView(texture, nullptr, &textureSRV);

	stbi_image_free(textureData);
	texture->Release();

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}