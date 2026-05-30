#pragma once
#include <string>


class CubemapTexture
{
public:
	CubemapTexture( std::string hdrPath );

	~CubemapTexture();
	
	void bind(unsigned int slot) const;
	
	void unbind() const;
	
	unsigned int getId() const { return textureID; }
	
	std::string getFilePath() const { return filePath; }
private:
	unsigned int textureID;
	std::string filePath;
};