#pragma once

#include "Mesh.h"
#include "Vector3.h"
#include "Matrix.h"

struct EngineObject 
{
    Mesh mesh;
    Vector3 position;

    EngineObject(Mesh mesh, Vector3 position) : mesh(mesh), position(position) {}
};