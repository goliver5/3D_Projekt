#pragma once
#include <vector>
#include <d3d11.h>
#include <string>



bool createTextures(ID3D11Device* device, ID3D11ShaderResourceView*& textureSRV, std::string fileName);