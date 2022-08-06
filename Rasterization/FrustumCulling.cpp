#include"FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
	rootNode = new Node();
	rootNode->isRootNode = true;

	//for (int i = 0; i < NROFCHILDREN; i++)
	//{
	//	//rootNode->demkids.push_back(new Node());
	//	createTree(rootNode, DEPTH);
	//}

	createTree(rootNode, DEPTH);

	createBoundingBoxes(rootNode, DEPTH, TOPLEFT, WIDTH, HEIGHT);
}

void FrustumCulling::createBoundingBoxes(Node* node, int depth, DirectX::XMFLOAT3 topLeft, float width, float height)
{

	float currentWidth = width / 2;
	float currentHeight = height / 2;

	if (depth > 0)
	{
		for (int i = 0; i < node->demkids.size(); i++)
		{

			if (i == 0)// 64,64 || 32,32
			{
				DirectX::XMFLOAT3 temp1(topLeft);
				DirectX::XMVECTOR point1 = DirectX::XMLoadFloat3(&temp1);
				DirectX::XMFLOAT3 temp2(topLeft.x - currentWidth, -topLeft.y, topLeft.z - currentHeight);
				DirectX::XMVECTOR point2 = DirectX::XMLoadFloat3(&temp2);

				DirectX::BoundingBox::CreateFromPoints(node->demkids[i]->boundingBox, point1, point2);

				createBoundingBoxes(node->demkids[i], depth - 1, temp1, currentWidth, currentHeight);
			}
			if (i == 1)// 32,64 || 0,32
			{
				DirectX::XMFLOAT3 temp1(topLeft.x - currentWidth, topLeft.y,topLeft.z);
				DirectX::XMVECTOR point1 = DirectX::XMLoadFloat3(&temp1);
				DirectX::XMFLOAT3 temp2(topLeft.x - width, -topLeft.y, topLeft.z - currentHeight);
				DirectX::XMVECTOR point2 = DirectX::XMLoadFloat3(&temp2);

				DirectX::BoundingBox::CreateFromPoints(node->demkids[i]->boundingBox, point1, point2);

				createBoundingBoxes(node->demkids[i], depth - 1, temp1, currentWidth, currentHeight);
			}

			if (i == 2)// 64,32 || 32,0
			{
				DirectX::XMFLOAT3 temp1(topLeft.x, topLeft.y, topLeft.z - currentHeight);
				DirectX::XMVECTOR point1 = DirectX::XMLoadFloat3(&temp1);
				DirectX::XMFLOAT3 temp2(topLeft.x - currentWidth, -topLeft.y, topLeft.z - height);
				DirectX::XMVECTOR point2 = DirectX::XMLoadFloat3(&temp2);

				DirectX::BoundingBox::CreateFromPoints(node->demkids[i]->boundingBox, point1, point2);

				createBoundingBoxes(node->demkids[i], depth - 1, temp1, currentWidth, currentHeight);
			}
			if (i == 3)// 32,32 || 0,0
			{
				DirectX::XMFLOAT3 temp1(topLeft.x - currentWidth, topLeft.y, topLeft.z - currentHeight);
				DirectX::XMVECTOR point1 = DirectX::XMLoadFloat3(&temp1);
				DirectX::XMFLOAT3 temp2(topLeft.x - width, -topLeft.y, topLeft.z - height);
				DirectX::XMVECTOR point2 = DirectX::XMLoadFloat3(&temp2);

				DirectX::BoundingBox::CreateFromPoints(node->demkids[i]->boundingBox, point1, point2);

				createBoundingBoxes(node->demkids[i], depth - 1, temp1, currentWidth, currentHeight);
			}
		}
	}

}

void FrustumCulling::createTree(Node* node, int depth)
{
	if (depth > 0)
	{
		for (int i = 0; i < NROFCHILDREN; i++)
		{
			node->demkids.push_back(new Node());
			createTree(node->demkids[i], depth - 1);
		}
	}
	//if (depth == 0)
	//{
	//	for (int i = 0; i < NROFCHILDREN; i++)
	//	{
	//		node->demkids.push_back(nullptr);
	//	}
	//}

}

void FrustumCulling::checkIntersectionAllObjects(Node* node, std::vector<SceneObject*>& allObjects)
{
	//node->objects.clear();
	bool isLeave = false;
	if (node->demkids.size() == 0)
	{
		isLeave = true;
	}
	if (isLeave)
	{
		//om inte lika med noll
		for (int i = 0; i < allObjects.size(); i++)
		{
			//int temp = node->boundingBox.Contains(allObjects[i]->getBoundingBox());
			int temp = node->boundingBox.Contains(allObjects[i]->getBoundingBox());

			if (!node->isRootNode)
			{
				OutputDebugString(std::to_wstring(temp).c_str());
				OutputDebugString(L"\n");
			}

			if (node->boundingBox.Contains(allObjects[i]->getBoundingBox()))
			{
				bool wow = true;
				node->objects.push_back(allObjects[i]);
				//this->currentScene.push_back(allObjects[i]);
			}
		}
	}
	else if(!isLeave)
	{
		for (int i = 0; i < node->demkids.size(); i++)
		{
			if (!isLeave)
			{
				checkIntersectionAllObjects(node->demkids[i], allObjects);
			}
		}
	}
}

void FrustumCulling::checkFrustumIntersectingNodes(Node* node, int depth, DirectX::BoundingFrustum& frustum)
{
	if (depth > 0)
	{
		for (int i = 0; i < NROFCHILDREN; i++)
		{
			checkFrustumIntersectingNodes(node->demkids[i], depth - 1,frustum);
		}
	}
	if (depth == 0)
	{
		if (node->boundingBox.Contains(frustum))
		{
			this->currentNodes.push_back(node);
		}
	}
}

void FrustumCulling::clearTreeRecursive(Node* node, int depth)
{
}


void FrustumCulling::culling(std::vector<SceneObject*> allObjects, Camera& currentCamera)
{
	this->checkIntersectionAllObjects(rootNode, allObjects);

	
}

void FrustumCulling::frustumCheck(Camera& currentCamera)
{
	//beräkna vilka noder som kamerans frustum intersectar med nodernas bounding boxes
	currentScene.clear();
	currentNodes.clear();
	DirectX::BoundingFrustum camFrustum = currentCamera.getFrustum();
	checkFrustumIntersectingNodes(rootNode, DEPTH, camFrustum);

	for (int i = 0; i < currentNodes.size(); i++)
	{
		for (int j = 0; j < currentNodes[i]->objects.size(); j++)
		{
			currentScene.push_back(currentNodes[i]->objects[j]);
		}
	}
}

void FrustumCulling::clearTree()
{
	clearTreeRecursive(rootNode, DEPTH);
}

std::vector<SceneObject*> FrustumCulling::getScene()
{
	return currentScene;
}
