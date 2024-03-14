#pragma once

struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

class VectorOperations
{

public:
	Vector3 Add(Vector3& v1, Vector3& v2)
	{
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	Vector3 Subtract(Vector3& v1, Vector3& v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3 Multiply(Vector3& v1, float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	Vector3 Divide(Vector3& v1, float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	float Dot(Vector3& v1, Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Length(Vector3& v)
	{
		return sqrtf(Dot(v, v));
	}

	Vector3 Normalise(Vector3& v)
	{
		float l = Length(v);
		return { v.x / l, v.y / l, v.z / l };
	}

	Vector3 Cross(Vector3& v1, Vector3& v2)
	{
		Vector3 v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	Vector3 IntersectPlane(Vector3& plane_p, Vector3& plane_n, Vector3& lineStart, Vector3& lineEnd)
	{
		plane_n = Normalise(plane_n);
		float plane_d = -Dot(plane_n, plane_p);
		float ad = Dot(lineStart, plane_n);
		float bd = Dot(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		Vector3 lineStartToEnd = Subtract(lineEnd, lineStart);
		Vector3 lineToIntersect = Multiply(lineStartToEnd, t);
		return Add(lineStart, lineToIntersect);
	}
};