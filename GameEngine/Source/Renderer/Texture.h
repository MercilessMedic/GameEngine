#pragma once

#include <iostream>
#include <assimp/scene.h>

using namespace std;

enum class TextureType
{
	//Phong model types
	DIFFUSE,
	SPECULAR,
	HEIGHT,
	
	//PBR model types
	ALBEDO,
	METALLIC,
	ROUGHNESS,
	AO,

	NORMAL
};

class Texture
{
public:

	Texture(const std::string path, TextureType type);
	~Texture(){printf("Texture destructor called!"); }
	void bind( unsigned int slot ) const;
	void unbind() const;

	unsigned int getId() const { return textureId; }
	TextureType getType() const { return type; }
	std::string getFilePath() const { return filePath; }
private:
	std::string filePath;
	TextureType type;
	unsigned int textureId;
	
};