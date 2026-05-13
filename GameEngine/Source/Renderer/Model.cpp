#include "Model.h"
#include "PBRMaterial.h"
#include "PhongMaterial.h"
#include "Shaders.h"

Model::Model(std::string path)
{
	loadModel( path );
}

void Model::loadModel( const std::string path )
{
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate |
													aiProcess_GenSmoothNormals |
													aiProcess_CalcTangentSpace |
													aiProcess_JoinIdenticalVertices);

	if( scene == nullptr || scene->mRootNode == nullptr ||
							scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cout << "[ERROR][MODEL] " << importer.GetErrorString();
	}
	else
	{
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//Process meshes of the node and add them to meshes vector.
	for(int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back( processMesh( mesh, scene ) );
	}
	//Process child nodes
	for(int i = 0; i < node->mNumChildren; i++)
	{
		processNode( node->mChildren[i], scene );
	}
}

Mesh Model::processMesh( aiMesh* mesh, const aiScene* scene )
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;
	unsigned int indexCount, vertexCount;


	//Process every vertex of the mesh
	//--------------------------------
	for(int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector3;
		glm::vec2 vector2;

		//Position vector
		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.Position = vector3;
		
		//Normal vector
		if( mesh->HasNormals() )
		{
			vector3.x = mesh->mNormals[i].x;
			vector3.y = mesh->mNormals[i].y;
			vector3.z = mesh->mNormals[i].z;
			vertex.Normal = vector3;
		}

		//Texture coordinates, Tangent and Bitanget vectors
		if( mesh->HasTextureCoords(0) )
		{
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector2;

			if( mesh->HasTangentsAndBitangents() )
			{
				vector3.x = mesh->mTangents[i].x;
				vector3.y = mesh->mTangents[i].y;
				vector3.z = mesh->mTangents[i].z;
				vertex.Tangent = vector3;

				vector3.x = mesh->mBitangents[i].x;
				vector3.y = mesh->mBitangents[i].y;
				vector3.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector3;

			}
		}
		else
		{ 
			vector2 = glm::vec2(0.0f, 0.0f);
			vector3 = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.TexCoords = vector2;
			vertex.Tangent = vector3;
			vertex.Bitangent = vector3;
		}
		//Initialize every bone id to -1
		memset(vertex.BoneIDs, -1, sizeof(vertex.BoneIDs));
		//Initialize evert bone weight to 0
		memset(vertex.BoneWeights, 0, sizeof(vertex.BoneWeights));
		vertices.push_back(vertex);
	}
	//Process Bones
	//-------------
	if (mesh->HasBones())
	{
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];
			std::string boneName = bone->mName.C_Str();
			int boneID;
			if (boneMap.find(boneName) == boneMap.end())
			{
				boneID = boneCounter;
				boneMap[boneName] = boneID;
				boneCounter++;

				BoneInfo info;
				info.boneID = boneID;
				info.offsetMatrix = ConvertMatrix(bone->mOffsetMatrix);
				bonesInfo.push_back(info);
			}
			else
			{
				boneID = boneMap[boneName];
			}

			for (int j = 0; j < bone->mNumWeights; j++)
			{
				aiVertexWeight weight = bone->mWeights[j];
				int vertexID = weight.mVertexId;
				float w = weight.mWeight;
				addBoneData(vertices[vertexID], boneID, w);
			}
		}
	}
	//Process faces of the mesh for indices
	//-------------------------------------
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for( int j = 0; j < face.mNumIndices; j++ )
		{
			indices.push_back( face.mIndices[j] );
		}
	}

	//Process the materials and load them
	//-----------------------------------
	aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
	float shininess;
	aiMaterial->Get(AI_MATKEY_SHININESS, shininess);

	std::shared_ptr<Material> material;

	bool hasPBR = aiMaterial->GetTextureCount(aiTextureType_METALNESS) > 0 ||
				  aiMaterial->GetTextureCount(aiTextureType_SHININESS) > 0;

	if( hasPBR )
	{
		//PBR
		auto pbrMat = std::make_shared<PBRMaterial>();
		pbrMat->albedoMap = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, TextureType::ALBEDO).empty() ? nullptr : 
															 loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, TextureType::ALBEDO)[0];

		pbrMat->normalMap = loadMaterialTextures(aiMaterial, aiTextureType_NORMALS, TextureType::NORMAL).empty() ? nullptr : 
															 loadMaterialTextures(aiMaterial, aiTextureType_NORMALS, TextureType::NORMAL)[0];

		pbrMat->metallicMap = loadMaterialTextures(aiMaterial, aiTextureType_METALNESS, TextureType::METALLIC).empty() ? nullptr : 
															 loadMaterialTextures(aiMaterial, aiTextureType_METALNESS, TextureType::METALLIC)[0];

		pbrMat->roughnessMap = loadMaterialTextures(aiMaterial, aiTextureType_SHININESS, TextureType::ROUGHNESS).empty() ? nullptr : 
															 loadMaterialTextures(aiMaterial, aiTextureType_SHININESS, TextureType::ROUGHNESS)[0];

		pbrMat->aoMap = loadMaterialTextures(aiMaterial, aiTextureType_AMBIENT_OCCLUSION, TextureType::AO).empty() ? nullptr : 
															loadMaterialTextures(aiMaterial, aiTextureType_AMBIENT_OCCLUSION, TextureType::AO)[0];
		material = pbrMat;
	}
	else
	{
		//PHONG
		auto phongMat = std::make_shared<PhongMaterial>();
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(aiMaterial, aiTextureType_SPECULAR, TextureType::SPECULAR);
		std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(aiMaterial, aiTextureType_NORMALS, TextureType::NORMAL);
		std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(aiMaterial, aiTextureType_HEIGHT, TextureType::HEIGHT);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		phongMat->setTextures(textures);
		float shininess;
		aiMaterial->Get(AI_MATKEY_SHININESS, shininess);
		if( shininess <= 0 ){ shininess = 32;}
		phongMat->setShininess( shininess );
		material = phongMat;
	}
	
	auto vertexArray = std::make_shared<VertexArray>(vertices, indices);

	return Mesh(vertexArray, material);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(
	aiMaterial* material, aiTextureType aiTexType, TextureType type)
{
	std::vector<std::shared_ptr<Texture>> textures;

	for (int i = 0; i < material->GetTextureCount(aiTexType); i++)
	{
		aiString str;
		material->GetTexture(aiTexType, i, &str);
		std::string filename = str.C_Str();
		std::string fullPath = this->directory + "/" + filename;
		bool loaded = false;

		for (auto& loadedTex : loadedTextures)  
		{
			if (fullPath == loadedTex->getFilePath())
			{
				textures.push_back(loadedTex);
				loaded = true;
				break;
			}
		}

		if (!loaded)
		{
			auto texture = std::make_shared<Texture>(fullPath, type);
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}

	return textures;
}

void Model::addBoneData( Vertex& vertex, int boneID, float weight )
{
	for( int i = 0; i < MAX_BONE_INFLUENCE; i++ )
	{
		if( vertex.BoneIDs[i] == -1 )
		{
			vertex.BoneIDs[i] = boneID;
			vertex.BoneWeights[i] = weight;
			return;
		}
	}
};

glm::mat4 Model::ConvertMatrix(const aiMatrix4x4& oldMat) 
{
	glm::mat4 newMat;

	newMat[0][0] = oldMat.a1; newMat[1][0] = oldMat.a2; newMat[2][0] = oldMat.a3; newMat[3][0] = oldMat.a4;
	newMat[0][1] = oldMat.b1; newMat[1][1] = oldMat.b2; newMat[2][1] = oldMat.b3; newMat[3][1] = oldMat.b4;
	newMat[0][2] = oldMat.c1; newMat[1][2] = oldMat.c2; newMat[2][2] = oldMat.c3; newMat[3][2] = oldMat.c4;
	newMat[0][3] = oldMat.d1; newMat[1][3] = oldMat.d2; newMat[2][3] = oldMat.d3; newMat[3][3] = oldMat.d4;

	return newMat;
}
