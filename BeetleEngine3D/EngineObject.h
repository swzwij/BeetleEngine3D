#pragma once

#include "Mesh.h"
#include "Vector3.h"
#include "Matrix.h"

struct EngineObject 
{
    string name;
    string tag;
    Mesh mesh;
    Vector3 position;
    Vector3 velocity;
    bool hasGravity;
    float bounciness;
    bool hasDrag;
    // radius

    EngineObject(string name, string tag, Mesh mesh, Vector3 position, Vector3 velocity, bool hasGravity, float bounciness, bool hasDrag) : name(name), tag(tag), mesh(mesh), position(position), velocity(velocity), hasGravity(hasGravity), bounciness(bounciness), hasDrag(hasDrag) {}
};