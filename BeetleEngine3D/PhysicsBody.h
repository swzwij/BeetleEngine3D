#pragma once

#include "Vector3.h"

struct PhysicsBody
{
	Vector3 velocity;

	float mass;
	float bounciness;

	bool hasGravity;
	bool hasDrag;
};