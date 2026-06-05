#pragma once
#include <string>


class CubemapTexture
{
public:
	CubemapTexture( std::string hdrPath );

	~CubemapTexture();
	
	void bind(unsigned int slot) const;
	
	void unbind() const;
	
	unsigned int gettextureID() const { return textureID; }
	unsigned int getTextureIrradianceID() const { return irradianceID; }
	
	std::string getFilePath() const { return filePath; }
private:
	unsigned int textureID;
	unsigned int irradianceID;
	std::string filePath;
};