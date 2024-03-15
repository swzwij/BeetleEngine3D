#include "ConsolseRenderingEngine.h"
#include "Vector3.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Color.h"
#include "EngineObject.h"
#include "Scene.h"

#include <fstream>
#include <strstream>
#include <algorithm>
#include <cmath>

using namespace std;

class BeetleEngine : public ConsolseRenderingEngine
{
	MatrixOperations matrixUtil;
	VectorOperations vectorUtil;
	TriangleOperations triangleUtil;
	ColorOperations colorUtil;
	Scene scene;

public:
	BeetleEngine()
	{
		m_sAppName = L"Beetle Engine 3D";
	}

private:

	Matrix4x4 projectionMatrix;
	Vector3 cameraPosition;
	Vector3 lookDirection;
	float cameraYaw;
	float cameraPitch;
	float theta;
	float gravity = -9.8f;
	bool wireFrame;
	std::vector<EngineObject> renderQueue;

	bool hasPhysics;

public:
	bool OnUserCreate() override
	{
		renderQueue = scene.Objects();

		projectionMatrix = matrixUtil.MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

		return true;
	}

	bool OnUserUpdate(float elapsedTime) override
	{
		UpdateCameraPosition(elapsedTime);

		PhysicsCalculations(elapsedTime);

		ClearScreen();

		ProcessRenderQueue(elapsedTime);
		
		return true;
	}

private:
	void UpdateCameraPosition(float elapsedTime)
	{
		if (GetKey(L'E').bHeld)
			cameraPosition.y += 8.0f * elapsedTime;

		if (GetKey(L'Q').bHeld)
			cameraPosition.y -= 8.0f * elapsedTime;

		Vector3 forward = vectorUtil.Multiply(lookDirection, 8.0f * elapsedTime);

		if (GetKey(L'W').bHeld)
			cameraPosition = vectorUtil.Add(cameraPosition, forward);

		if (GetKey(L'S').bHeld)
			cameraPosition = vectorUtil.Subtract(cameraPosition, forward);

		if (GetKey(L'A').bHeld)
			cameraYaw -= 2.0f * elapsedTime;

		if (GetKey(L'D').bHeld)
			cameraYaw += 2.0f * elapsedTime;

		if (GetKey(L'K').bHeld)
			cameraPitch -= 2.0f * elapsedTime;

		if (GetKey(L'L').bHeld)
			cameraPitch += 2.0f * elapsedTime;

		if (GetKey(L'P').bPressed)
			hasPhysics = !hasPhysics;

		if (GetKey(L'T').bPressed)
			wireFrame = !wireFrame;
	}

	void PhysicsCalculations(float elapsedTime)
	{
		if (!hasPhysics)
			return;

		for (auto& engineObject : renderQueue)
		{
			if (engineObject.tag == "GROUND")
				continue;

			// Apply gravity
			if (engineObject.hasGravity)
				engineObject.velocity.y += gravity * elapsedTime;

			// Add velocity
			Vector3 velocity = engineObject.velocity;
			velocity.x *= elapsedTime;
			velocity.y *= elapsedTime;
			velocity.z *= elapsedTime;
			Vector3 newPosition = vectorUtil.Add(engineObject.position, velocity);
			engineObject.position = newPosition;

			// apply drag
			if (engineObject.hasDrag) 
			{
				float dragCoefficient = 0.25f;  // For a sphere
				float airDensity = 1.225f;     // Approximate density of air
				float area = 1.0f;             // Estimate - depends on object shape

				// Calculate drag force
				Vector3 normalizedVelocity = vectorUtil.Normalise(engineObject.velocity);
				float magnitudeSquaredVelocity = vectorUtil.MagnitudeSquared(engineObject.velocity);
				Vector3 normalizedMagnitudeSquaredVelocity = vectorUtil.Multiply(normalizedVelocity, magnitudeSquaredVelocity);
				Vector3 dragForce = vectorUtil.Multiply(normalizedMagnitudeSquaredVelocity, (-0.5f * dragCoefficient * airDensity * area));

				// Apply drag force as an acceleration
				//Vector3 dragAcceleration = dragForce; // TODO: divide by mass
				Vector3 dragAcceleration = vectorUtil.Multiply(dragForce, elapsedTime);
				engineObject.velocity = vectorUtil.Add(engineObject.velocity, dragAcceleration);
			}

			// Ground Check
			float sphereRadius = 1;
			Vector3 groundPoint = { 0, 0, 0 };
			Vector3 groundNormal = { 0, 1, 0 };
			float groundDistance = (newPosition.x - groundPoint.x) * groundNormal.x +
				(newPosition.y - groundPoint.y) * groundNormal.y +
				(newPosition.z - groundPoint.z) * groundNormal.z;

			// Ground collision
			if (groundDistance <= sphereRadius)
			{
				engineObject.position.y += (sphereRadius - fabs(groundDistance));
				engineObject.velocity.y = -engineObject.velocity.y * engineObject.bounciness;
			}

			for (auto& otherEngineObject : renderQueue)
			{
				if (otherEngineObject.name == engineObject.name)
					continue;

				// Collision check
				Vector3 thisPosition = engineObject.position;
				Vector3 otherPosition = otherEngineObject.position;
				float sphereDistance = sqrtf((otherPosition.x - thisPosition.x) * (otherPosition.x - thisPosition.x) +
					(otherPosition.y - thisPosition.y) * (otherPosition.y - thisPosition.y) +
					(otherPosition.z - thisPosition.z) * (otherPosition.z - thisPosition.z));

				// Collision
				if (sphereDistance <= sphereRadius * 2)
				{
					// Calculate collision normal
					Vector3 collisionDifference = vectorUtil.Subtract(otherPosition, thisPosition);
					Vector3 collisionNormal = vectorUtil.Normalise(collisionDifference);

					// Store original relative velocity
					Vector3 originalRelativeVelocity = vectorUtil.Subtract(engineObject.velocity, otherEngineObject.velocity);

					// Calculate coefficient of restitution effect
					float coefficient_of_restitution = 0.8;
					Vector3 normalRelativeVelocity = vectorUtil.Multiply(collisionNormal, vectorUtil.Dot(collisionNormal, originalRelativeVelocity));
					Vector3 coefficient = vectorUtil.Multiply(normalRelativeVelocity, (1 + coefficient_of_restitution));
					Vector3 newRelativeVelocity = vectorUtil.Subtract(originalRelativeVelocity, coefficient);

					// Calculate impulse
					float totalMass = 1 + 1; // Assuming masses are 1
					Vector3 impulse = vectorUtil.Multiply(newRelativeVelocity, totalMass);

					// Apply collision impulse
					engineObject.velocity.x = (engineObject.velocity.x + impulse.x) / totalMass;
					engineObject.velocity.y = (engineObject.velocity.y + impulse.y) / totalMass;
					engineObject.velocity.z = (engineObject.velocity.z + impulse.z) / totalMass;

					otherEngineObject.velocity.x = (otherEngineObject.velocity.x - impulse.x) / totalMass;
					otherEngineObject.velocity.y = (otherEngineObject.velocity.y - impulse.y) / totalMass;
					otherEngineObject.velocity.z = (otherEngineObject.velocity.z - impulse.z) / totalMass;

					// Displacement 
					float overlap = (sphereRadius + sphereRadius) - sphereDistance;
					Vector3 displacementSphere = vectorUtil.Multiply(collisionNormal, overlap / 2);

					engineObject.position = vectorUtil.Subtract(engineObject.position, displacementSphere);
					otherEngineObject.position = vectorUtil.Add(otherEngineObject.position, displacementSphere);
				}
			}
		}
	}

	void ClearScreen()
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
	}

	void ProcessRenderQueue(float elapsedTime)
	{
		for (auto& engineObject : renderQueue)
		{
			// World Tranmsform
			Matrix4x4 matRotZ, matRotX;
			//theta += 2.0f * elapsedTime;
			matRotZ = matrixUtil.MakeRotationZ(theta * 0.5f);
			matRotX = matrixUtil.MakeRotationX(theta * 0.25f);

			Matrix4x4 matTrans;
			matTrans = matrixUtil.MakeTranslation(engineObject.position.x, engineObject.position.y, engineObject.position.z);

			Matrix4x4 matWorld;
			matWorld = matrixUtil.MakeIdentity();	// Form World Matrix
			matWorld = matrixUtil.MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
			matWorld = matrixUtil.MultiplyMatrix(matWorld, matTrans); // Transform by translation

			// Create point tt Matrix for camera
			Vector3 vectorUp = { 0,1,0 };
			Vector3 vectorForward = { 0,0,1 };
			Matrix4x4 cameraRotationMatrix = matrixUtil.MakeRotationY(cameraYaw);
			lookDirection = matrixUtil.MultiplyVector(cameraRotationMatrix, vectorForward);
			vectorForward = vectorUtil.Add(cameraPosition, lookDirection);
			Matrix4x4 cameraMatrix = matrixUtil.PointAt(cameraPosition, vectorForward, vectorUp);

			// Make view matrix from camera
			Matrix4x4 matView = matrixUtil.QuickInverse(cameraMatrix);

			// Store triagles for rastering later
			vector<Triangle> vecTrianglesToRaster;

			// Draw Triangles
			for (auto tri : engineObject.mesh.tris)
			{
				Triangle projectedTri, transformedTri, viewedTri;

				// World Matrix Transform
				transformedTri.verticies[0] = matrixUtil.MultiplyVector(matWorld, tri.verticies[0]);
				transformedTri.verticies[1] = matrixUtil.MultiplyVector(matWorld, tri.verticies[1]);
				transformedTri.verticies[2] = matrixUtil.MultiplyVector(matWorld, tri.verticies[2]);

				// Calculate triangle Normal
				Vector3 normal;
				Vector3 line1;
				Vector3 line2;

				// lines either side of triangle
				line1 = vectorUtil.Subtract(transformedTri.verticies[1], transformedTri.verticies[0]);
				line2 = vectorUtil.Subtract(transformedTri.verticies[2], transformedTri.verticies[0]);

				// Take cross product of lines to get normal to triangle surface
				normal = vectorUtil.Cross(line1, line2);

				// You normally need to normalise a normal!
				normal = vectorUtil.Normalise(normal);

				// Get Ray from triangle to camera
				Vector3 cameraRay = vectorUtil.Subtract(transformedTri.verticies[0], cameraPosition);

				// If ray is aligned with normal, then triangle is visible
				if (vectorUtil.Dot(normal, cameraRay) < 0.0f)
				{
					// Illumination
					Vector3 light_direction = { 0.0f, 1.0f, -1.0f };
					light_direction = vectorUtil.Normalise(light_direction);

					// How "aligned" are light direction and triangle surface normal?
					float dp = max(0.1f, vectorUtil.Dot(light_direction, normal));

					CHAR_INFO color = colorUtil.GetColour(dp);
					transformedTri.color = color.Attributes;
					transformedTri.symbol = color.Char.UnicodeChar;

					// Convert World Space to View Space
					viewedTri.verticies[0] = matrixUtil.MultiplyVector(matView, transformedTri.verticies[0]);
					viewedTri.verticies[1] = matrixUtil.MultiplyVector(matView, transformedTri.verticies[1]);
					viewedTri.verticies[2] = matrixUtil.MultiplyVector(matView, transformedTri.verticies[2]);
					viewedTri.symbol = transformedTri.symbol;
					viewedTri.color = transformedTri.color;

					// Clip Viewed Triangle against near plane, this could form two additional
					// additional triangles. 
					int clippedTriangles = 0;
					Triangle clipped[2];
					clippedTriangles = triangleUtil.ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, viewedTri, clipped[0], clipped[1]);

					for (int n = 0; n < clippedTriangles; n++)
					{
						// Project triangles from 3D 2D
						projectedTri.verticies[0] = matrixUtil.MultiplyVector(projectionMatrix, clipped[n].verticies[0]);
						projectedTri.verticies[1] = matrixUtil.MultiplyVector(projectionMatrix, clipped[n].verticies[1]);
						projectedTri.verticies[2] = matrixUtil.MultiplyVector(projectionMatrix, clipped[n].verticies[2]);
						projectedTri.color = clipped[n].color;
						projectedTri.symbol = clipped[n].symbol;

						// Scale into view
						projectedTri.verticies[0] = vectorUtil.Divide(projectedTri.verticies[0], projectedTri.verticies[0].w);
						projectedTri.verticies[1] = vectorUtil.Divide(projectedTri.verticies[1], projectedTri.verticies[1].w);
						projectedTri.verticies[2] = vectorUtil.Divide(projectedTri.verticies[2], projectedTri.verticies[2].w);

						// X/Y are inverted so put them back
						projectedTri.verticies[0].x *= -1.0f;
						projectedTri.verticies[1].x *= -1.0f;
						projectedTri.verticies[2].x *= -1.0f;
						projectedTri.verticies[0].y *= -1.0f;
						projectedTri.verticies[1].y *= -1.0f;
						projectedTri.verticies[2].y *= -1.0f;

						// Offset verts into visible normalised space
						Vector3 vOffsetView = { 1,1,0 };
						projectedTri.verticies[0] = vectorUtil.Add(projectedTri.verticies[0], vOffsetView);
						projectedTri.verticies[1] = vectorUtil.Add(projectedTri.verticies[1], vOffsetView);
						projectedTri.verticies[2] = vectorUtil.Add(projectedTri.verticies[2], vOffsetView);
						projectedTri.verticies[0].x *= 0.5f * (float)ScreenWidth();
						projectedTri.verticies[0].y *= 0.5f * (float)ScreenHeight();
						projectedTri.verticies[1].x *= 0.5f * (float)ScreenWidth();
						projectedTri.verticies[1].y *= 0.5f * (float)ScreenHeight();
						projectedTri.verticies[2].x *= 0.5f * (float)ScreenWidth();
						projectedTri.verticies[2].y *= 0.5f * (float)ScreenHeight();

						// Store triangle for sorting
						vecTrianglesToRaster.push_back(projectedTri);
					}
				}
			}

			// Sort triangles from back to front
			sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](Triangle& t1, Triangle& t2)
			{
				float z1 = (t1.verticies[0].z + t1.verticies[1].z + t1.verticies[2].z) / 3.0f;
				float z2 = (t2.verticies[0].z + t2.verticies[1].z + t2.verticies[2].z) / 3.0f;
				return z1 > z2;
			});

			// Loop through all transformed, viewed, projected, and sorted triangles
			for (auto& triToRaster : vecTrianglesToRaster)
			{
				// Clip triangles against all four screen edges.
				Triangle clipped[2];
				list<Triangle> listTriangles;

				// Add initial triangle
				listTriangles.push_back(triToRaster);
				int newTriangles = 1;

				for (int p = 0; p < 4; p++)
				{
					int trisToAdd = 0;
					while (newTriangles > 0)
					{
						// Take triangle from front of queue
						Triangle test = listTriangles.front();
						listTriangles.pop_front();
						newTriangles--;

						// Clip it against a plane. We only need to test each 
						// subsequent plane, against subsequent new triangles
						// as all triangles after a plane clip are guaranteed
						// to lie on the inside of the plane.
						switch (p)
						{
						case 0:	trisToAdd = triangleUtil.ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
						case 1:	trisToAdd = triangleUtil.ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
						case 2:	trisToAdd = triangleUtil.ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
						case 3:	trisToAdd = triangleUtil.ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
						}

						// Clipping may yield a variable number of triangles, so
						// add these new ones to the back of the queue for subsequent
						// clipping against next planes
						for (int w = 0; w < trisToAdd; w++)
							listTriangles.push_back(clipped[w]);
					}
					newTriangles = listTriangles.size();
				}


				// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
				for (auto& t : listTriangles)
				{
					if (wireFrame)
						DrawTriangle(t.verticies[0].x, t.verticies[0].y, t.verticies[1].x, t.verticies[1].y, t.verticies[2].x, t.verticies[2].y, PIXEL_SOLID, FG_GREEN);
					else
						FillTriangle(t.verticies[0].x, t.verticies[0].y, t.verticies[1].x, t.verticies[1].y, t.verticies[2].x, t.verticies[2].y, t.symbol, t.color);
				}
			}
		}

	}
};

int main()
{
	BeetleEngine engine;

	if (engine.ConstructConsole(256, 240, 4, 4))
		engine.Start();

	return 0;
}