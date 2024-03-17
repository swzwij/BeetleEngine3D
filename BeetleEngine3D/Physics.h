#pragma once
#include "EngineObject.h"
#include "Vector3.h"

class Physics
{
	VectorOperations vectorUtil;

private:
	float _frictionCoefficient = -0.5f;
	float _dragCoefficient = 0.25f;
	float _restitutionCoefficient = 0.8; // Amount of velocity transfer
	float _airDensity = 1.225f;
	float _area = 1.0f; // Estimate - depends on object shape

public:
	void ApplyGravity(EngineObject& obj, float gravity, float elapsedTime)
	{
		obj.velocity.y += gravity * elapsedTime;
	}

	void ApplyVelocity(EngineObject& obj, float elapsedTime)
	{
		Vector3 velocity = obj.velocity;
		velocity = vectorUtil.Multiply(velocity, elapsedTime);
		Vector3 newPosition = vectorUtil.Add(obj.position, velocity);
		obj.position = newPosition;
	}

	void ApplyDrag(EngineObject& obj, float elapsedTime)
	{
		// Calculate drag force
		Vector3 normalizedVelocity = vectorUtil.Normalise(obj.velocity);
		float magnitudeSquaredVelocity = vectorUtil.MagnitudeSquared(obj.velocity);
		Vector3 normalizedMagnitudeSquaredVelocity = vectorUtil.Multiply(normalizedVelocity, magnitudeSquaredVelocity);
		Vector3 dragForce = vectorUtil.Multiply(normalizedMagnitudeSquaredVelocity, (-0.5f * _dragCoefficient * _airDensity * _area));

		// Apply drag force as an acceleration
		Vector3 dragAcceleration = vectorUtil.Divide(dragForce, obj.mass);
		Vector3 scaledDragAcceleration = vectorUtil.Multiply(dragAcceleration, elapsedTime);
		obj.velocity = vectorUtil.Add(obj.velocity, scaledDragAcceleration);
	}

	float GroundDistance(EngineObject& obj)
	{
		Vector3 groundPoint = { 0, 0, 0 }; // TODO: Fix hardcoded ground.
		Vector3 groundNormal = { 0, 1, 0 }; // TODO: Fix hardcoded ground.

		float groundDistance = 
			(obj.position.x - groundPoint.x) * groundNormal.x + 
			(obj.position.y - groundPoint.y) * groundNormal.y + 
			(obj.position.z - groundPoint.z) * groundNormal.z;

		return groundDistance;
	}

	void HandleGroundCollision(EngineObject& obj, float groundDistance, float gravity, float elapsedTime)
	{
		obj.position.y += (obj.radius - fabs(groundDistance));
		obj.velocity.y = -obj.velocity.y * obj.bounciness;

		// Apply friction 
		if (vectorUtil.MagnitudeSquared(obj.velocity) > 0.01f) // Check if object is moving
		{
			ApplyFriction(obj, gravity, elapsedTime);
		}

		if (vectorUtil.MagnitudeSquared(obj.velocity) < 0.001f)
		{
			obj.velocity = { 0, 0, 0 };
		}
	}

	void ApplyFriction(EngineObject& obj, float gravity, float elapsedTime)
	{
		// TODO: objects "never" stop

		float normalForce = obj.mass * gravity;
		float frictionForce = _frictionCoefficient * normalForce;

		// Direction opposite to velocity 
		Vector3 frictionDirection = vectorUtil.Normalise(obj.velocity);
		frictionDirection = vectorUtil.Multiply(frictionDirection, -1.0f); // Reverse direction

		Vector3 frictionAcceleration = vectorUtil.Multiply(frictionDirection, (frictionForce / obj.mass));

		Vector3 frictionAccelerationScaled = vectorUtil.Multiply(frictionAcceleration, elapsedTime);
		obj.velocity = vectorUtil.Add(obj.velocity, frictionAccelerationScaled); // Update velocity (additive change)
	}

	void CalculateSphereCollision(EngineObject& objA, EngineObject& objB)
	{
		// Collision check
		Vector3 thisPosition = objA.position;
		Vector3 otherPosition = objB.position;
		float sphereDistance = sqrtf
		(
			(otherPosition.x - thisPosition.x) * (otherPosition.x - thisPosition.x) +
			(otherPosition.y - thisPosition.y) * (otherPosition.y - thisPosition.y) +
			(otherPosition.z - thisPosition.z) * (otherPosition.z - thisPosition.z)
		);

		// Collision
		if (sphereDistance <= (objA.radius + objB.radius))
		{
			// Calculate collision normal
			Vector3 collisionDifference = vectorUtil.Subtract(otherPosition, thisPosition);
			Vector3 collisionNormal = vectorUtil.Normalise(collisionDifference);

			// Store original relative velocity
			Vector3 originalRelativeVelocity = vectorUtil.Subtract(objA.velocity, objB.velocity);

			// Calculate coefficient of restitution effect
			Vector3 normalRelativeVelocity = vectorUtil.Multiply(collisionNormal, vectorUtil.Dot(collisionNormal, originalRelativeVelocity));
			Vector3 coefficient = vectorUtil.Multiply(normalRelativeVelocity, (1 + _restitutionCoefficient));
			Vector3 newRelativeVelocity = vectorUtil.Subtract(originalRelativeVelocity, coefficient);

			// Calculate impulse
			float totalMass = objA.mass + objB.mass;
			Vector3 impulse = vectorUtil.Multiply(newRelativeVelocity, totalMass);

			// Apply collision impulse
			objA.velocity.x = (objA.velocity.x + impulse.x) / totalMass;
			objA.velocity.y = (objA.velocity.y + impulse.y) / totalMass;
			objA.velocity.z = (objA.velocity.z + impulse.z) / totalMass;

			objB.velocity.x = (objB.velocity.x - impulse.x) / totalMass;
			objB.velocity.y = (objB.velocity.y - impulse.y) / totalMass;
			objB.velocity.z = (objB.velocity.z - impulse.z) / totalMass;

			// Displacement 
			float overlap = (objA.radius + objB.radius) - sphereDistance;
			Vector3 displacementSphere = vectorUtil.Multiply(collisionNormal, overlap / 2);

			objA.position = vectorUtil.Subtract(objA.position, displacementSphere);
			objB.position = vectorUtil.Add(objB.position, displacementSphere);
		}
	}
};