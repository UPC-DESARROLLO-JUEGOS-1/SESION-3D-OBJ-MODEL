#pragma once

#include <vector>
#include <string>
#include "VertexData.h"


struct dtMeshData {
	V3 Position;
	V3 Normal;
	V2 TextCoord;
};

struct dtModelObjMesh
{
	std::vector<dtMeshData> Vertices;
	std::vector<unsigned int> Triangles;
	std::string texturePath;
	V3 DiffuseColor;
};