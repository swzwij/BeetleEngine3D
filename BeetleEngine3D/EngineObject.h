#pragma once

#include "Mesh.h"
#include "Vector3.h"
#include "Matrix.h"

struct EngineObject 
{
    Mesh mesh;
    Vector3 position;
    Vector3 velocity;

    EngineObject(Mesh mesh, Vector3 position, Vector3 velocity) : mesh(mesh), position(position), velocity(velocity) {}
};