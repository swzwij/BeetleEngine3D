#pragma once
#include "Vector3.h"

struct Matrix4x4
{
	float elements[4][4] = { 0 };
};

class MatrixOperations
{
	VectorOperations vectorUtil;

public:

	Vector3 MultiplyVector(Matrix4x4& m, Vector3& v)
	{
		Vector3 result;
		result.x = v.x * m.elements[0][0] + v.y * m.elements[1][0] + v.z * m.elements[2][0] + v.w * m.elements[3][0];
		result.y = v.x * m.elements[0][1] + v.y * m.elements[1][1] + v.z * m.elements[2][1] + v.w * m.elements[3][1];
		result.z = v.x * m.elements[0][2] + v.y * m.elements[1][2] + v.z * m.elements[2][2] + v.w * m.elements[3][2];
		result.w = v.x * m.elements[0][3] + v.y * m.elements[1][3] + v.z * m.elements[2][3] + v.w * m.elements[3][3];
		return result;
	}

	Matrix4x4 MakeIdentity()
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = 1.0f;
		matrix.elements[1][1] = 1.0f;
		matrix.elements[2][2] = 1.0f;
		matrix.elements[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 MakeRotationX(float angleRad)
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = 1.0f;
		matrix.elements[1][1] = cosf(angleRad);
		matrix.elements[1][2] = sinf(angleRad);
		matrix.elements[2][1] = -sinf(angleRad);
		matrix.elements[2][2] = cosf(angleRad);
		matrix.elements[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 MakeRotationY(float fAngleRad)
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = cosf(fAngleRad);
		matrix.elements[0][2] = sinf(fAngleRad);
		matrix.elements[2][0] = -sinf(fAngleRad);
		matrix.elements[1][1] = 1.0f;
		matrix.elements[2][2] = cosf(fAngleRad);
		matrix.elements[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 MakeRotationZ(float fAngleRad)
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = cosf(fAngleRad);
		matrix.elements[0][1] = sinf(fAngleRad);
		matrix.elements[1][0] = -sinf(fAngleRad);
		matrix.elements[1][1] = cosf(fAngleRad);
		matrix.elements[2][2] = 1.0f;
		matrix.elements[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 MakeTranslation(float x, float y, float z)
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = 1.0f;
		matrix.elements[1][1] = 1.0f;
		matrix.elements[2][2] = 1.0f;
		matrix.elements[3][3] = 1.0f;
		matrix.elements[3][0] = x;
		matrix.elements[3][1] = y;
		matrix.elements[3][2] = z;
		return matrix;
	}

	Matrix4x4 MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		Matrix4x4 matrix;
		matrix.elements[0][0] = fAspectRatio * fFovRad;
		matrix.elements[1][1] = fFovRad;
		matrix.elements[2][2] = fFar / (fFar - fNear);
		matrix.elements[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.elements[2][3] = 1.0f;
		matrix.elements[3][3] = 0.0f;
		return matrix;
	}

	Matrix4x4 MultiplyMatrix(Matrix4x4& m1, Matrix4x4& m2)
	{
		Matrix4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.elements[r][c] = m1.elements[r][0] * m2.elements[0][c] + m1.elements[r][1] * m2.elements[1][c] + m1.elements[r][2] * m2.elements[2][c] + m1.elements[r][3] * m2.elements[3][c];
		return matrix;
	}

	Matrix4x4 PointAt(Vector3& pos, Vector3& target, Vector3& up)
	{
		// Calculate new forward direction
		Vector3 newForward = vectorUtil.Subtract(target, pos);
		newForward = vectorUtil.Normalise(newForward);

		// Calculate new Up direction
		Vector3 a = vectorUtil.Multiply(newForward, vectorUtil.Dot(up, newForward));
		Vector3 newUp = vectorUtil.Subtract(up, a);
		newUp = vectorUtil.Normalise(newUp);

		// New Right direction is easy, its just cross product
		Vector3 newRight = vectorUtil.Cross(newUp, newForward);

		// Construct Dimensioning and Translation Matrix	
		Matrix4x4 matrix;
		matrix.elements[0][0] = newRight.x;	matrix.elements[0][1] = newRight.y;	matrix.elements[0][2] = newRight.z;	matrix.elements[0][3] = 0.0f;
		matrix.elements[1][0] = newUp.x;		matrix.elements[1][1] = newUp.y;		matrix.elements[1][2] = newUp.z;		matrix.elements[1][3] = 0.0f;
		matrix.elements[2][0] = newForward.x;	matrix.elements[2][1] = newForward.y;	matrix.elements[2][2] = newForward.z;	matrix.elements[2][3] = 0.0f;
		matrix.elements[3][0] = pos.x;			matrix.elements[3][1] = pos.y;			matrix.elements[3][2] = pos.z;			matrix.elements[3][3] = 1.0f;
		return matrix;

	}

	Matrix4x4 QuickInverse(Matrix4x4& m) // Only for Rotation/Translation Matrices
	{
		Matrix4x4 matrix;
		matrix.elements[0][0] = m.elements[0][0]; matrix.elements[0][1] = m.elements[1][0]; matrix.elements[0][2] = m.elements[2][0]; matrix.elements[0][3] = 0.0f;
		matrix.elements[1][0] = m.elements[0][1]; matrix.elements[1][1] = m.elements[1][1]; matrix.elements[1][2] = m.elements[2][1]; matrix.elements[1][3] = 0.0f;
		matrix.elements[2][0] = m.elements[0][2]; matrix.elements[2][1] = m.elements[1][2]; matrix.elements[2][2] = m.elements[2][2]; matrix.elements[2][3] = 0.0f;
		matrix.elements[3][0] = -(m.elements[3][0] * matrix.elements[0][0] + m.elements[3][1] * matrix.elements[1][0] + m.elements[3][2] * matrix.elements[2][0]);
		matrix.elements[3][1] = -(m.elements[3][0] * matrix.elements[0][1] + m.elements[3][1] * matrix.elements[1][1] + m.elements[3][2] * matrix.elements[2][1]);
		matrix.elements[3][2] = -(m.elements[3][0] * matrix.elements[0][2] + m.elements[3][1] * matrix.elements[1][2] + m.elements[3][2] * matrix.elements[2][2]);
		matrix.elements[3][3] = 1.0f;
		return matrix;
	}
};