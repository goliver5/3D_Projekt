#include"FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
	rootNode = new Node();

	for (int i = 0; i < NROFCHILDREN; i++)
	{
		rootNode->demkids.push_back(new Node());
		createTree(rootNode->demkids[i],3);
	}
	
}

void FrustumCulling::createTree(Node* node, int depth)
{
	if (depth > 0)
	{
		for (int i = 0; i < NROFCHILDREN; i++)
		{
			node->demkids.push_back(new Node());
			createTree(node->demkids[i], --depth);
		}
	}
}

bool FrustumCulling::checkifObjectExist(Node node, SceneObject* object)
{
	return false;
}