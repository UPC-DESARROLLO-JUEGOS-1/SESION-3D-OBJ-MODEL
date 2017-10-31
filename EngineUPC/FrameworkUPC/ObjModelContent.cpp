#include "ObjModelContent.h"
#include "OBJ_Loader.h"

ObjModelContent::ObjModelContent(ContentManager* contentManager)
	: BaseContent(contentManager)
{
}

ObjModelContent::~ObjModelContent() {}

void ObjModelContent::Initialize()
{
	BaseContent::Initialize();
}

bool ObjModelContent::Load(const std::string path)
{
	BaseContent::Load(path);

	int lastIndex = path.find_first_of('/', 0);
	std::string folderPath = lastIndex != -1 ? path.substr(0, lastIndex + 1) : "";

	// Initialize Loader
	objl::Loader* objLoader = new objl::Loader();
	// Load .obj File
	bool loadout = objLoader->LoadFile(path);

	if (loadout) {
		for (int i = 0; i < objLoader->LoadedMeshes.size(); i++)
		{
			objl::Mesh currentMesh = objLoader->LoadedMeshes[i];

			dtModelObjMesh mesh;
			mesh.texturePath = currentMesh.MeshMaterial.map_Kd;
			mesh.DiffuseColor.x = currentMesh.MeshMaterial.Kd.X;
			mesh.DiffuseColor.y = currentMesh.MeshMaterial.Kd.Y;
			mesh.DiffuseColor.z = currentMesh.MeshMaterial.Kd.Z;

			// Load Texture
			ImageContent* imageContent = contentManager->LoadAndGetContent<ImageContent>(folderPath + mesh.texturePath);
			mImageContents.push_back(imageContent);

			for (int j = 0; j < currentMesh.Vertices.size(); j++) {
				objl::Vertex vertex = currentMesh.Vertices[j];

				dtMeshData newVertex;
				newVertex.Position.x = vertex.Position.X;
				newVertex.Position.y = vertex.Position.Y;
				newVertex.Position.z = vertex.Position.Z;
				newVertex.Normal.x = vertex.Normal.X;
				newVertex.Normal.y = vertex.Normal.Y;
				newVertex.Normal.z = vertex.Normal.Z;
				newVertex.TextCoord.x = vertex.TextureCoordinate.X;
				newVertex.TextCoord.y = 1.0f - vertex.TextureCoordinate.Y;

				mesh.Vertices.push_back(newVertex);
			}

			for (int j = 0; j < currentMesh.Indices.size(); j += 3) {
				mesh.Triangles.push_back(currentMesh.Indices[j]);
				mesh.Triangles.push_back(currentMesh.Indices[j + 1]);
				mesh.Triangles.push_back(currentMesh.Indices[j + 2]);
			}

			mMeshes.push_back(mesh);
		}
	}

	objLoader->~Loader();
	objLoader = nullptr;

	return loadout;
}