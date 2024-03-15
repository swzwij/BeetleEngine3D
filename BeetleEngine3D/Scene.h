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

		sceneObjects.push_back(CreateObject("ground.obj", { 0, 0, 0 }, { 0, 0, 0 }));

		sceneObjects.push_back(CreateObject("sphere.obj", { 0, 10, 5 }, { 0, -2, 0 }));
		sceneObjects.push_back(CreateObject("sphere.obj", { 5, 12, 5 }, { 0, -2, 0 }));

		return sceneObjects;
	}

private:
	EngineObject CreateObject(string objectFileName, Vector3 position, Vector3 velocity)
	{
		Mesh objectMesh;
		objectMesh.LoadObjectFile(objectFileName);
		EngineObject engineObject(objectMesh, position, velocity);
		return engineObject;
	}
};