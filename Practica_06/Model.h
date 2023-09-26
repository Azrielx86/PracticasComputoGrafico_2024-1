#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

class Model
{
  public:
	Model();

	void LoadModel(const std::string &fileName);
	void RenderModel();
	void ClearModel();

	~Model();

  private:
	void LoadNode(aiNode *node, const aiScene *scene); // assimp
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);
	std::vector<Mesh *> MeshList;
	std::vector<Texture *> TextureList;
	std::vector<unsigned int> meshTotex;
};
