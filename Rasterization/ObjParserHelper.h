#pragma once
#include "objectStruct.h"
#include <string>

struct objectStruct
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<VertexData> vertexForIndex;
	std::vector<int> vertexSubMeshCounter;
	std::vector<int> indices;
};

void ObjParserHelper(std::vector<objectStruct>& objectInformation);
