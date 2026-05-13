#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

struct BoneInfo
{
	int boneID;
	glm::mat4 offsetMatrix;
};

class Model
{
public:
	
	Model(std::string path);

	~Model() {};

	int getNumMeshes() const { return meshes.size(); }

	const std::vector<Mesh>& getMeshes() const { return meshes; }

private:
	std::string directory;
	std::vector<Mesh> meshes;
	int boneCounter = 0;
	std::vector<BoneInfo> bonesInfo;
	std::map<string, int> boneMap;
	std::vector<std::shared_ptr<Texture>> loadedTextures;	
	glm::mat4 ConvertMatrix(const aiMatrix4x4& mat);
	
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void addBoneData( Vertex& vertex, int boneID, float weight);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType aiTexType,
		TextureType type);
};