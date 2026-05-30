#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>


Texture::Texture(const std::string path, TextureType textureType, bool isHDR):
	textureID(0), filePath(path), type(textureType)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	
	//data is for normal textures and hdrData for HDR textures
	unsigned char* data = nullptr;
	float* hdrData = nullptr;
	
	//Load the image data
	//-------------------
	if( isHDR )
	{
		 hdrData = stbi_loadf( path.c_str(), &width, &height, &nrChannels, 0);
	}
	else
	{
		 data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	}
	
	if( !data && !hdrData )
	{
		std::cout << "[WARNING] [TEXTURE]Failed to load texture with path:" << path << std::endl;
		return;	
	}

	//Create texture object and bind it
	//---------------------------------
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum format = GL_RGB;
	GLenum internalFormat = GL_RGB8;
	GLint level = 0;
	GLint border = 0;

	switch( nrChannels )
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	
	//Upload the image data
	//--------------------
	if( isHDR )
	{
		internalFormat = (nrChannels == 3) ? GL_RGB16F :
						 (nrChannels == 4) ? GL_RGBA16F: GL_R16F;
		glTexImage2D
		(
			GL_TEXTURE_2D, 
			level,
			internalFormat, 
			width, height, 
			border,
			format, 
			GL_FLOAT, 
			hdrData 
		);
		//Wrapping and filtering parameters for HDR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexImage2D
		(
			GL_TEXTURE_2D, 
			level, 
			internalFormat, 
			width, height, 
			border, 
			format, 
			GL_UNSIGNED_BYTE, 
			data
		);
		glGenerateMipmap(GL_TEXTURE_2D);

		//wrapping and filtering parameters for normal textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	stbi_image_free( data );
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind(unsigned int slot) const
{
	//Activate the texture slot
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, textureID);
}
void Texture::unbind() const
{
	glActiveTexture( 0 );
}