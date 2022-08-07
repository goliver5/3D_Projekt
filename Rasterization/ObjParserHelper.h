#pragma once
#include "objectStruct.h"
#include "OBJParser.h"
#include <d3d11.h>
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
};

void ObjParserHelper(std::vector<objectStruct>& output, ID3D11Device* device);
