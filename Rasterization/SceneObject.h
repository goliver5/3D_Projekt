#pragma once
#include "objectStruct.h"
#include "OBJParser.h"
#include "ConstantBufferNew.h"
#include <d3d11.h>

class SceneObject
{
private:
	std::vector<VertexData> vertexData;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer* indexBuffer;
	std::vector<float>wow;
	ConstantBufferNew<WMatrix> constantBuffer;
	DirectX::XMMATRIX world;

	std::vector<VertexData> vertexForIndex;
	std::vector<int>vertexSubMeshCounter;
	std::vector<int> indices;

	ID3D11ShaderResourceView* textureSRV;
	/*VertexData triangle[6] =
	{
		{ {-0.5f, 0.5f, 0.0f}, {0, 0, -1}, {0, 0} },
		{ {0.5f, -0.5f, 0.0f}, {0, 0, -1}, {1, 1} },
		{ {-0.5f, -0.5f, 0.0f}, {0, 0, -1}, {0, 1} },

		{ {-0.5f, 0.5f, 0.0f}, {0, 0, -1}, {0, 0} },
		{ {0.5f, 0.5f, 0.0f}, {0, 0, -1}, {1, 0} },
		{ {0.5f, -0.5f, 0.0f}, {0, 0, -1}, {1, 1} }
	};*/
protected:
	HRESULT createVertexBuffer(ID3D11Device* device);
	bool createIndexBuffer(ID3D11Device* device);
public:
	SceneObject(ID3D11Device* device, ID3D11DeviceContext* immediateContext, ID3D11ShaderResourceView*& textureSRVs, std::string fileName);


	void setGroundPos();
	void rotateObject(float x, float y, float z);
	void tempUpdate();
	void noMemoryLeak();

	void draw(ID3D11DeviceContext*& immediateContext);
};