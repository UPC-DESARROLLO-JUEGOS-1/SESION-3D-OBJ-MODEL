#pragma once

#include "Drawable3D.h"
#include "VertexData.h"
#include "ImageContent.h"
#include "ImageContent.h"
#include "dtModelObjMesh.h"

#include <vector>

class NMeshRenderer3D : public Drawable3D
{
public:
	NMeshRenderer3D() { Drawable3D::Drawable3D(); }
	~NMeshRenderer3D() { Drawable3D::~Drawable3D(); }

	void Initialize(float x, float y, float z);

	void Begin();
	void PushVertex(float x, float y, float z);
	void PushVertex(V3 position, V3 normal, V2 uv);
	void PushArrayVertex(std::vector<dtMeshData> vertices);
	void PushTriangle(unsigned int i0, unsigned int i1, unsigned int i2);
	void PushTriangles(std::vector<unsigned int> triangles);
	void End(V3 diffuseColor);
	void End(ImageContent* content);
	void Draw(float dt);

protected:
	std::vector<dtMeshData> mVertices;
	std::vector<unsigned int> mIndices;

	V3 AmbientColor;

	BaseShader* mTextureShader;
	BaseShader* mPrimitiveShader;
};

