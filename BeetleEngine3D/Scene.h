#pragma once

#include "EngineObject.h"
#include "Vector3.h"
#include "Mesh.h"

#include <vector>

using namespace std;

class Scene
{
public:
	std::vector<EngineObject> Objects()
	{
		std::vector<EngineObject> sceneObjects;

		sceneObjects.push_back(CreateObject("ground", "GROUND", "ground.obj", { 0, 0, 0 }, { 0, 0, 0 }, false, 0, false));

		// Side / top collision
		/*sceneObjects.push_back(CreateObject("sphere1", "SPHERE", "sphere.obj", { 0, 1.5, 5 }, { 0, 0, 0 }, true, 0.25f, true));
		sceneObjects.push_back(CreateObject("sphere2", "SPHERE", "sphere.obj", { 5, 3, 5 }, { -10, 0, 0 }, true, 0.25f, true));*/

		// Head on collison
		sceneObjects.push_back(CreateObject("sphere1", "SPHERE", "sphere.obj", { 2.5, 1.5, 5 }, { -10, 0, 0 }, true, 0.25f, true));
		sceneObjects.push_back(CreateObject("sphere2", "SPHERE", "sphere.obj", { -2.5, 1.5, 5 }, { 10, 0, 0 }, true, 0.25f, true));

		sceneObjects.push_back(CreateObject("sphere3", "SPHERE", "sphere.obj", { 5, 1.5, 5 }, { 0, 0, 0 }, true, 0.25f, true));
		sceneObjects.push_back(CreateObject("sphere4", "SPHERE", "sphere.obj", { -5, 1.5, 0 }, { 0, 0, 10 }, true, 0.25f, true));

		return sceneObjects;
	}

private:
	EngineObject CreateObject(string name, string tag, string objectFileName, Vector3 position, Vector3 velocity, bool hasGravity, float bounciness, bool hasDrag)
	{
		Mesh objectMesh;
		objectMesh.LoadObjectFile(objectFileName);
		EngineObject engineObject(name, tag, objectMesh, position, velocity, hasGravity, bounciness, hasDrag);
		return engineObject;
	}
};