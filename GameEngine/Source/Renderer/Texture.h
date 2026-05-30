#pragma once

#include <iostream>

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

	Texture(const std::string path, TextureType type, bool isHDR = false);
	~Texture();
	void bind( unsigned int slot ) const;
	void unbind() const;

	unsigned int getId() const { return textureID; }
	TextureType getType() const { return type; }
	std::string getFilePath() const { return filePath; }
private:
	std::string filePath;
	TextureType type;
	unsigned int textureID;
	
};