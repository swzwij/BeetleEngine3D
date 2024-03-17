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

		sceneObjects.push_back(CreateObject("ground", "GROUND", "ground.obj", false));

		// Side / top collision
		/*sceneObjects.push_back(CreateObject("sphere1", "SPHERE", "sphere.obj", { 0, 1.5, 5 }, { 0, 0, 0 }, true, 0.25f, true));
		sceneObjects.push_back(CreateObject("sphere2", "SPHERE", "sphere.obj", { 5, 3, 5 }, { -10, 0, 0 }, true, 0.25f, true));*/

		// Head on collison
		sceneObjects.push_back(CreateObject("sphere1", "SPHERE", "sphere.obj", true, { 2.5, 1.5f, 5 }, { -10, 0, 0 }));
		sceneObjects.push_back(CreateObject("sphere2", "SPHERE", "sphere.obj", true, { -2.5, 1.5f, 5 }, { 10, 0, 0 }));

		sceneObjects.push_back(CreateObject("sphere3", "SPHERE", "sphere.obj", true, { 5, 1.5f, 5 }));
		sceneObjects.push_back(CreateObject("sphere4", "SPHERE", "sphere.obj", true, { -5, 1.5f, 0 }, { 0, 0, 10 }));
		sceneObjects.push_back(CreateObject("sphere5", "SPHERE", "sphere.obj", true, { 2.5f, 5, 5 }));
		sceneObjects.push_back(CreateObject("sphere6", "SPHERE", "sphere.obj", true, { -2.5f, 5, 5 }));

		return sceneObjects;
	}

private:
	EngineObject CreateObject(string name, string tag, string objectFileName, bool isSimulated = true, Vector3 position = {0, 0, 0}, Vector3 velocity = { 0, 0, 0 }, float radius = 1.0f, float mass = 1.0f, float bounciness = 0.25f, bool hasGravity = true, bool hasDrag = true)
	{
		Mesh objectMesh;
		objectMesh.LoadObjectFile(objectFileName);
		EngineObject engineObject(name, tag, objectMesh, isSimulated, position, velocity, radius, mass, bounciness, hasGravity, hasDrag);
		return engineObject;
	}
};