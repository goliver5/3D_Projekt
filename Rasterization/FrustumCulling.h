#pragma once
#include <vector>
#include "SceneObject.h"
#include <DirectXCollision.h>

const UINT NROFCHILDREN = 4;

struct Node
{
	std::vector<Node*> demkids;
	std::vector<SceneObject*> objects;
	DirectX::BoundingBox boundingBox;
};

class FrustumCulling
{
private:
	Node* rootNode;

	void createTree(Node* node, int depth);
public:
	FrustumCulling();

	bool checkifObjectExist(Node node, SceneObject *object);
};