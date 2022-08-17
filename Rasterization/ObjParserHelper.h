#pragma once
#include "objectStruct.h"
#include "OBJParser.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

struct objectStruct
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<VertexData> vertexForIndex;
	std::vector<int> vertexSubMeshCounter;
	std::vector<int> indices;
	std::vector<ID3D11ShaderResourceView*> srvs_ka;
	std::vector<ID3D11ShaderResourceView*> srvs_kd;
	std::vector<ID3D11ShaderResourceView*> srvs_ks;
	DirectX::XMFLOAT3 topLeft;
	DirectX::XMFLOAT3 bottomLeft;
	float NS;//Shininiess

	void noMemoryLeaks()
	{
		for (int i = 0; i < srvs_kd.size(); i++)
		{
			 srvs_kd[i]->Release();
		}
		for (int i = 0; i < srvs_ka.size(); i++)
		{
			srvs_ka[i]->Release();
		}
		for (int i = 0; i < srvs_ks.size(); i++)
		{
			 srvs_ks[i]->Release();
		}
	};
};

void ObjParserHelper(std::vector<objectStruct>& output, ID3D11Device* device);
