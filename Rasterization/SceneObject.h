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
	std::vector<float>wow;
	ConstantBufferNew<WMatrix> constantBuffer;
	DirectX::XMMATRIX world;
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
public:
	SceneObject(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	void tempUpdate();
	
	void draw(ID3D11DeviceContext*& immediateContext);
};