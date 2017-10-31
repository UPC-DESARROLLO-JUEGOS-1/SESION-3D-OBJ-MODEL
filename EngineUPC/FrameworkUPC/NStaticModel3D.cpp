#include "NStaticModel3D.h"

#include "GameFramework.h"

NStaticModel3D::NStaticModel3D()
{
}

void NStaticModel3D::Initialize(std::string modelPath, float x, float y, float z) {
	
	int lastIndex = modelPath.find_first_of('/', 0);
	mFolder = lastIndex != -1 ? modelPath.substr(0, lastIndex + 1) : "";

	mContentManager = GameFramework::GET_FRAMEWORK()->GetContentManager();

	mModelContent = mContentManager->LoadAndGetContent<ObjModelContent>(modelPath);
	mMeshes = mModelContent->GetMeshes();

	 // Initialize Mesh Renderer
	mMeshRenderer = new NMeshRenderer3D();
	mMeshRenderer->Initialize(x, y, z);
	mMeshRenderer->SetRenderCamera(mRenderCamera);
}

void NStaticModel3D::Update(float dt) {
	mMeshRenderer->Update(dt);
}

void NStaticModel3D::Draw(float dt) {
	for (int i = 0, count = mMeshes.size(); i < count; i++)
	{
		dtModelObjMesh mesh = mMeshes[i];

		mMeshRenderer->Begin();
		mMeshRenderer->PushArrayVertex(mesh.Vertices); // mandamos vertices
		mMeshRenderer->PushTriangles(mesh.Triangles); // mandamos indices

		if (mesh.texturePath.size() > 0) {
			ImageContent* content = mContentManager->LoadAndGetContent<ImageContent>(
				mFolder + mesh.texturePath);
			mMeshRenderer->End(content);
		}
		else {
			mMeshRenderer->End(mesh.DiffuseColor);
		}
		
		mMeshRenderer->Draw(dt);
	}
}