#pragma once

#include "BaseContent.h"
#include "ContentManager.h"

#include "dtModelObjMesh.h"

class ObjModelContent : public BaseContent
{
public:
	ObjModelContent(ContentManager* contentManager);
	~ObjModelContent();

	std::vector<dtModelObjMesh> GetMeshes() { return mMeshes; }
	std::vector<ImageContent*> GetImageContents() { return mImageContents; }

	void Initialize();
	bool Load(const std::string path);

private:
	std::vector<dtModelObjMesh> mMeshes;
	std::vector<ImageContent*> mImageContents;
};

