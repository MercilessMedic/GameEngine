#pragma once

#include <glm/glm.hpp>
#define MAX_BONE_INFLUENCE 4

//Vertex attribute indices
enum class VertAttrib: unsigned int
{
	POSITION	 = 0,
	NORMAL		 = 1,
	TEXCOORDS	 = 2,
	TANGENT		 = 3,
	BITANGENT	 = 4,
	BONE_IDS	 = 5,
	BONE_WEIGHTS = 6
};

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int BoneIDs[MAX_BONE_INFLUENCE];
	float BoneWeights[MAX_BONE_INFLUENCE];
};

