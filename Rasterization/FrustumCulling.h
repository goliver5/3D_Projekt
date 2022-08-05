#pragma once
#include <vector>
#include "SceneObject.h"
#include <DirectXCollision.h>

const UINT NROFCHILDREN = 4;
const int DEPTH = 3;

const float WIDTH = 64.0f;
const float HEIGHT = 64.0f;
const DirectX::XMFLOAT3 TOPLEFT = DirectX::XMFLOAT3(WIDTH, 40.0f, HEIGHT);
//const DirectX::XMFLOAT3 TOPLEFTZ = DirectX::XMFLOAT3(WIDTH, 40.0f, -HEIGHT);

struct Node
{
	std::vector<Node*> demkids;
	std::vector<SceneObject*> objects;
	DirectX::BoundingBox boundingBox;
	//Node()
	//{
	//	for (int i = 0; i < NROFCHILDREN; i++)
	//	{
	//		demkids.push_back(nullptr);
	//	}
	//}
};

class FrustumCulling
{
private:
	Node* rootNode;	

	void createBoundingBoxes(Node* node, int depth, DirectX::XMFLOAT3 topLeft, float width, float height);
	void createTree(Node* node, int depth);
public:
	FrustumCulling();

	DirectX::XMFLOAT3 getBBPpositions();
	bool checkifObjectExist(Node node, SceneObject *object);
};