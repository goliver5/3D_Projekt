#pragma once
#include"SceneObject.h"
#include <vector>

class Scene
{
private:
	std::vector<SceneObject> *sceneObjects;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
public:
	Scene(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void addObject(SceneObject*& sceneObject);
	void addObject(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void update();
	void draw(ID3D11DeviceContext* immediateContext);
};
