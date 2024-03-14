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

		sceneObjects.push_back(CreateObject("sphere.obj", { 0, 0, 5 }));

		return sceneObjects;
	}

private:
	EngineObject CreateObject(string objectFileName, Vector3 position)
	{
		Mesh objectMesh;
		objectMesh.LoadObjectFile(objectFileName);
		EngineObject engineObject(objectMesh, position);
		return engineObject;
	}
};