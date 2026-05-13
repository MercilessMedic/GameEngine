#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>


Texture::Texture(const std::string path, TextureType textureType):
	textureId(0), filePath(path), type(textureType)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;

	glGenTextures(1, &textureId);
	unsigned char* data = stbi_load( path.c_str(), &width, &height, &nrChannels, 0);
	if(data)
	{
		GLenum format = GL_RGB;
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
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "\n[WARNING][TEXTURE]Failed to load texture with path:" << path << std::endl;
	}

	//wrapping and filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free( data );
}

void Texture::bind(unsigned int slot) const
{
	//Activate the texture slot
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, textureId );
}
void Texture::unbind() const
{
	glActiveTexture( 0 );
}