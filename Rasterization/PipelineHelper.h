#pragma once

#include <array>
#include <d3d11.h>
#include <DirectXMath.h>


struct SimpleVertex
{
	float pos[3];
	float nrm[3];
	float uv[2];

	SimpleVertex(const std::array<float, 3>& position, const std::array<float, 3> normal, const std::array<float, 2> uvs)
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

//void CreateTexture2Ds(ID3D11Device* device);
bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
	ID3D11SamplerState*& sampler, ID3D11ComputeShader*& cShader, ID3D11GeometryShader*& geometryShader);