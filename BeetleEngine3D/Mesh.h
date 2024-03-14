#pragma once
#include <fstream>
#include <strstream>
#include <vector>
#include "Triangle.h"
using namespace std;

struct Mesh
{
	vector<Triangle> tris;

	bool LoadObjectFile(string filename)
	{
		ifstream input(filename);
		if (!input.is_open())
			return false;

		vector<Vector3> vertices;

		while (!input.eof())
		{
			char line[128];
			input.getline(line, 128);

			strstream lineStream;
			lineStream << line;

			char junk;

			if (line[0] == 'v')
			{
				Vector3 vertex;
				lineStream >> junk >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}

			if (line[0] == 'f')
			{
				int faceIndices[3];
				lineStream >> junk >> faceIndices[0] >> faceIndices[1] >> faceIndices[2];
				tris.push_back({ vertices[faceIndices[0] - 1], vertices[faceIndices[1] - 1], vertices[faceIndices[2] - 1] });
			}
		}
		return true;
	}
};