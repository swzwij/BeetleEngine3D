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
    
    float radius;

    bool isSimulated;

    Vector3 velocity;

    float mass;
    float bounciness;

    bool hasGravity;
    bool hasDrag;

    EngineObject
    (
        string name,
        string tag,
        Mesh mesh,
        bool isSimulated,
        Vector3 position = { 0, 0, 0 },
        Vector3 velocity = { 0, 0, 0 },
        float radius = 1.0f,
        float mass = 1.0f,
        float bounciness = 0.25f,
        bool hasGravity = true,
        bool hasDrag = true
    ) : 
        name(name),
        tag(tag),
        mesh(mesh),
        isSimulated(isSimulated),
        position(position),
        velocity(velocity),
        radius(radius),
        mass(mass),
        bounciness(bounciness),
        hasGravity(hasGravity),
        hasDrag(hasDrag) 
    { }
};