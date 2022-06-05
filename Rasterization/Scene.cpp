#include "Scene.h"

Scene::Scene(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	this->device = device;
	this->immediateContext = immediateContext;
}

void Scene::addObject(SceneObject*& sceneObject)
{
	sceneObjects->push_back(*sceneObject);
}

void Scene::addObject(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	//sceneObjects->push_back(SceneObject(device, immediateContext, "beel.png"));
}

void Scene::update()
{
	for (int i = 0; i < sceneObjects->size(); i++)
	{
		sceneObjects->at(i).tempUpdate();
	}
}

void Scene::draw(ID3D11DeviceContext* immediateContext)
{
	for (int i = 0; i < sceneObjects->size(); i++)
	{
		sceneObjects->at(i).draw(immediateContext);
	}
}
