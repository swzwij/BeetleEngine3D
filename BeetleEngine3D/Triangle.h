#pragma once
#include "Vector3.h"

struct Triangle
{
	Vector3 verticies[3];
	wchar_t symbol;
	short color;
};


class TriangleOperations
{
	VectorOperations vectorUtil;

public:
	int ClipAgainstPlane(Vector3 clippingPlanePoint, Vector3 clippingPlaneNormal, Triangle& inputTriangle, Triangle& outputTriangleA, Triangle& outputTriangleB)
	{
		clippingPlaneNormal = vectorUtil.Normalise(clippingPlaneNormal);

		auto distanceToPlane = [&](Vector3& point)
		{
			return 
			(
				clippingPlaneNormal.x * point.x +
				clippingPlaneNormal.y * point.y +
				clippingPlaneNormal.z * point.z -
				vectorUtil.Dot(clippingPlaneNormal, clippingPlanePoint)
			);
		};

		Vector3* insidePoints[3];  
		int insidePointCount = 0;

		Vector3* outsidePoints[3]; 
		int outsidePointCount = 0;

		float distanceA = distanceToPlane(inputTriangle.verticies[0]);
		float distanceB = distanceToPlane(inputTriangle.verticies[1]);
		float distanceC = distanceToPlane(inputTriangle.verticies[2]);

		if (distanceA >= 0) 
			insidePoints[insidePointCount++] = &inputTriangle.verticies[0];
		else 
			outsidePoints[outsidePointCount++] = &inputTriangle.verticies[0];

		if (distanceB >= 0) 
			insidePoints[insidePointCount++] = &inputTriangle.verticies[1];
		else 
			outsidePoints[outsidePointCount++] = &inputTriangle.verticies[1];

		if (distanceC >= 0) 
			insidePoints[insidePointCount++] = &inputTriangle.verticies[2];
		else 
			outsidePoints[outsidePointCount++] = &inputTriangle.verticies[2];

		if (insidePointCount == 0)
			return 0;

		if (insidePointCount == 3)
		{
			outputTriangleA = inputTriangle;
			return 1; 
		}

		if (insidePointCount == 1 && outsidePointCount == 2)
		{
			outputTriangleA.color = inputTriangle.color;
			outputTriangleA.symbol = inputTriangle.symbol;

			outputTriangleA.verticies[0] = *insidePoints[0];

			outputTriangleA.verticies[1] = vectorUtil.IntersectPlane(clippingPlanePoint, clippingPlaneNormal, *insidePoints[0], *outsidePoints[0]);
			outputTriangleA.verticies[2] = vectorUtil.IntersectPlane(clippingPlanePoint, clippingPlaneNormal, *insidePoints[0], *outsidePoints[1]);

			return 1;
		}

		if (insidePointCount == 2 && outsidePointCount == 1)
		{
			outputTriangleA.color = inputTriangle.color;
			outputTriangleA.symbol = inputTriangle.symbol;

			outputTriangleB.color = inputTriangle.color;
			outputTriangleB.symbol = inputTriangle.symbol;

			outputTriangleA.verticies[0] = *insidePoints[0];
			outputTriangleA.verticies[1] = *insidePoints[1];
			outputTriangleA.verticies[2] = vectorUtil.IntersectPlane(clippingPlanePoint, clippingPlaneNormal, *insidePoints[0], *outsidePoints[0]);

			outputTriangleB.verticies[0] = *insidePoints[1];
			outputTriangleB.verticies[1] = outputTriangleA.verticies[2];
			outputTriangleB.verticies[2] = vectorUtil.IntersectPlane(clippingPlanePoint, clippingPlaneNormal, *insidePoints[1], *outsidePoints[0]);

			return 2;
		}
	}
};