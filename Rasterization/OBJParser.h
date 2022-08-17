#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include "objectStruct.h"

using namespace std;

bool ParseOBJFile(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& uvs, std::vector<VertexData> &vertexForIndex,
	std::vector<int> &vertexSubMeshCounter, std::vector<int> &indices, string fileName, ID3D11Device* device, std::vector<ID3D11ShaderResourceView*>& srvs_ka,
	std::vector<ID3D11ShaderResourceView*>& srvs_kd, std::vector<ID3D11ShaderResourceView*>& srvs_ks, DirectX::XMFLOAT3& topLeft, DirectX::XMFLOAT3& bottomLeft, std::vector<float>& NS);