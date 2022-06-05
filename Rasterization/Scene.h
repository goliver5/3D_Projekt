#pragma once
#include"SceneObject.h"
#include <vector>

class Scene
{
private:
	std::vector<SceneObject> sceneObjects;
public:

	void update();
	void draw();
};
