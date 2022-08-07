#pragma once
#include <vector>
#include "SceneObject.h"
#include "Camera.h"
#include <DirectXCollision.h>

const UINT NROFCHILDREN = 4;
const int DEPTH = 3;

const float WIDTH = 128.0f;
const float HEIGHT = 128.0f;
const DirectX::XMFLOAT3 TOPLEFT = DirectX::XMFLOAT3(WIDTH/2, 40.0f, HEIGHT/2);
//const DirectX::XMFLOAT3 TOPLEFTZ = DirectX::XMFLOAT3(WIDTH, 40.0f, -HEIGHT);

struct Node
{
	std::vector<Node*> demkids;
	std::vector<SceneObject*> objects;
	DirectX::BoundingBox boundingBox;
	bool isRootNode = false;
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

	std::vector<Node*> currentNodes;
	std::vector<SceneObject*> currentScene;

	//for deletion
	std::vector<Node*> allNodes;

	void createBoundingBoxes(Node* node, int depth, DirectX::XMFLOAT3 topLeft, float width, float height);
	void createTree(Node* node, int depth);
	void checkIntersectionAllObjects(Node* rootNode, std::vector<SceneObject*>& allObjects);
	void checkFrustumIntersectingNodes(Node* node, int depth, DirectX::BoundingFrustum& frustum);
	void clearTreeRecursive(Node* node, int depth);
	bool ifExists(std::vector<SceneObject*> objects, SceneObject* compareObject);
	//std::vector<Node*> getIntersectingNodes(Node* rootnode);
public:
	FrustumCulling();
	~FrustumCulling();

	void culling(std::vector<SceneObject*> allObjects, Camera& currentCamera);
	void frustumCheck(Camera& currentCamera);
	void clearTree();
	std::vector<SceneObject*> getScene();
};