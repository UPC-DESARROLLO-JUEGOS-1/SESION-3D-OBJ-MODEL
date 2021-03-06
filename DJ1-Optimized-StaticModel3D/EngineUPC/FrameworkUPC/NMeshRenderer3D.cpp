#include "NMeshRenderer3D.h"
#include "GameFramework.h"
#include "ShaderManager.h"
#include "ModelShader3D.h"
#include "ModelPrimitiveShader3D.h"

void NMeshRenderer3D::Initialize(float x, float y, float z)
{
	Drawable3D::Initialize(x, y, z);

	GameFramework* framework = GameFramework::GET_FRAMEWORK();
	ShaderManager* shdMng = framework->GetShaderManager();

	const std::string path_Texture = "Shaders/ModelShader3D";
	const std::string path_Primitive = "Shaders/ModelPrimitiveShader3D";

	mTextureShader = (BaseShader*)shdMng->LoadAndGetShader<ModelShader3D>(path_Texture);
	mPrimitiveShader = (BaseShader*)shdMng->LoadAndGetShader<ModelPrimitiveShader3D>(path_Primitive);
}

void NMeshRenderer3D::Begin() {
	mVertices.clear();
	//mIndices.clear();

	AmbientColor.x = 1;
	AmbientColor.y = 1;
	AmbientColor.z = 1;
}

void NMeshRenderer3D::PushVertex(float x, float y, float z) {
	
}

void NMeshRenderer3D::PushVertex(V3 position, V3 normal, V2 uv) {
	dtMeshData meshData;
	meshData.Position = position;
	meshData.Normal = normal;
	meshData.TextCoord = uv;

	mVertices.push_back(meshData);
}

void NMeshRenderer3D::PushArrayVertex(std::vector<dtMeshData> vertices) {
	mVertices = vertices;
}

void NMeshRenderer3D::PushTriangle(unsigned int i0, unsigned int i1, unsigned int i2) {

}

void NMeshRenderer3D::PushTriangles(std::vector<unsigned int> triangles) {
	mIndices = triangles;
}

void NMeshRenderer3D::End(V3 diffuseColor) {
	AmbientColor = diffuseColor;

	// For: Vertices
	int size = (sizeof(float) * 8) * mVertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, &mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// For: Indices
	int indexDataSize = sizeof(int) * mIndices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, &(mIndices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mIndicesCount = mIndices.size();

	// clear texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// set primtiive shader
	mCurrentShader = mPrimitiveShader;
}

void NMeshRenderer3D::End(ImageContent* content) {
	// For: Vertices
	int size = (sizeof(float) * 8) * mVertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, &mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// For: Indices
	int indexDataSize = sizeof(int) * mIndices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, &(mIndices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mIndicesCount = mIndices.size();

	// set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, content->GetTextureId());
	// set texture shader
	mCurrentShader = mTextureShader;
}

void NMeshRenderer3D::Draw(float dt) {
	mCurrentShader->Use();

	GLuint mvpLocation = mCurrentShader->GetUniformLocation("wvp");
	GLuint ambientColor = mCurrentShader->GetUniformLocation("ambientColor");
	glm::mat4 cameraMatrix = mRenderCamera->GetResultMatrix();
	glm::mat4 resultMatrix = cameraMatrix * worldMatrix;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(resultMatrix[0][0]));
	glUniform4f(ambientColor, AmbientColor.x, AmbientColor.y, AmbientColor.z, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);

	glEnable(GL_DEPTH_TEST);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dtMeshData), 
		(void*)offsetof(dtMeshData, dtMeshData::Position));
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(dtMeshData),
		(void*)offsetof(dtMeshData, dtMeshData::Normal));
	// TextureCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(dtMeshData),
		(void*)offsetof(dtMeshData, dtMeshData::TextCoord));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO_ID);

	// Esto dibuja en Frame Buffer
	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_DEPTH_TEST);

	mCurrentShader->Stop();
}