#pragma once
#include "ConsolseRenderingEngine.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Matrix.h"
#include "Color.h"
#include "Vector3.h"

#include <algorithm>

class MeshRenderer
{
	MatrixOperations matrixUtil;
	VectorOperations vectorUtil;
	TriangleOperations triangleUtil;
	ColorOperations colorUtil;

public:
	void Render(Mesh mesh)
	{
		return;
	}
};