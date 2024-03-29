#pragma once
#include "objectStruct.h"
#include "OBJParser.h"
#include "ObjParserHelper.h"
#include "ConstantBufferNew.h"
#include <d3d11.h>
#include <DirectXCollision.h>


class SceneObject
{
private:
	std::vector<VertexData> vertexData;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer* indexBuffer;
	std::vector<float>wow;
	ConstantBufferNew<WMatrix> constantBuffer;
	ConstantBufferNew<Shiny> nsBuffer;
	std::vector<float> NS;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX bbWorldMatrix;

	std::vector<VertexData> vertexForIndex;
	std::vector<int> vertexSubMeshCounter;
	std::vector<int> indices;

	std::vector<ID3D11ShaderResourceView*> kdSrv;
	std::vector<ID3D11ShaderResourceView*> kaSrv;
	std::vector<ID3D11ShaderResourceView*> ksSrv;


	DirectX::BoundingBox bBox;

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
	SceneObject(ID3D11Device* device, ID3D11DeviceContext* immediateContext, objectStruct objectData);


	void setGroundPos();
	void setPosition(float x, float y, float z);
	void rotateObject(float x, float y, float z);
	void tempUpdate();
	void noMemoryLeak();

	void update();
	void flyLeft();

	DirectX::BoundingBox &getBoundingBox();

	void draw(ID3D11DeviceContext*& immediateContext);
	void drawCubeMap(ID3D11DeviceContext*& immediateContext);
};