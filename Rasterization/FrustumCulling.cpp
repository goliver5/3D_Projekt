#include"FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
	rootNode = new Node();

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

			if (i == 1)// 64,64 || 32,32
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

}

void FrustumCulling::checkIntersectionAllObjects(Node* node, std::vector<SceneObject*> allObjects)
{

	bool isLeave = false;
	if (node->demkids.size() == 0)
	{
		isLeave = true;
	}
	for (int i = 0; i < NROFCHILDREN; i++)
	{
		if (!isLeave)
		{
			checkIntersectionAllObjects(node->demkids[i], allObjects);
		}
		if (isLeave)
		{
			//om inte lika med noll
			int temp = node->boundingBox.Contains(allObjects[i]->getBoundingBox());
			if(temp != 0)
			{
				bool wow = true;
				node->objects.push_back(allObjects[i]);
			}
		}
	}
}


void FrustumCulling::culling(std::vector<SceneObject*> allObjects)
{
	this->checkIntersectionAllObjects(rootNode, allObjects);
}

void FrustumCulling::getCulledTree()
{

}

std::vector<SceneObject*> FrustumCulling::getScene()
{

	return rootNode->demkids[0]->demkids[1]->demkids[1]->objects;
}
