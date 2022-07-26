#pragma once
#include <vector>

struct VertexData
{
	float pos[3];
	float nrm[3];
	float uv[2];

	VertexData(const std::vector<float> position, const std::vector<float> normal, const std::vector<float> uvs)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			nrm[i] = normal[i];
		}
		for (int i = 0; i < 2; i++)
		{
			uv[i] = uvs[i];
		}
	}

};