#include "CubemapTexture.h"
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Shaders.h"


static void RenderBakingCube()
{
	static unsigned int cubeVAO = 0;
	static unsigned int cubeVBO = 0;

	if (cubeVAO == 0)
	{
		float vertices[] = 
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,

			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,

			 -1.0f, -1.0f, -1.0f,
			  1.0f, -1.0f, -1.0f,
			  1.0f, -1.0f,  1.0f,
			  1.0f, -1.0f,  1.0f,
			 -1.0f, -1.0f,  1.0f,
			 -1.0f, -1.0f, -1.0f,

			 -1.0f,  1.0f,  1.0f,
			  1.0f,  1.0f,  1.0f,
			  1.0f,  1.0f, -1.0f,
			  1.0f,  1.0f, -1.0f,
			 -1.0f,  1.0f, -1.0f,
			 -1.0f,  1.0f,  1.0f
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);
	}

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

CubemapTexture::CubemapTexture(std::string path)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	float* hdrData = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);

	unsigned int hdrTexture;
	if (hdrData)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, hdrData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(hdrData);
	}
	else
	{
		std::cout << "[WARNING] [CUBEMAPTEXTURE] Failed to load HDR texture with the path:" << path << std::endl;
		return;
	}

	//create frame buffer object and render buffer object
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers( 1, &captureFBO );
	glGenRenderbuffers( 1, &captureRBO );

	glBindFramebuffer( GL_FRAMEBUFFER, captureFBO );
	glBindRenderbuffer( GL_RENDERBUFFER, captureRBO );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO );

	//create the cubemap
	unsigned int cubemap;
	glGenTextures( 1, &cubemap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap);

	GLint level = 0;
	GLint border = 0;
	for( int i = 0; i < 6; i++ )
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, GL_RGB16F, 512, 512, border, GL_RGB, GL_FLOAT, nullptr);
	}

	//Filterng and wrapping parameters for the cube	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//We set up projection matrix that with 90 degree field of view to capture entire face and
	//6 different view matrices which all face each side of the cube
	glm::mat4 captureProject = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	Shaders::EquirectangularToCubemap->use();
	Shaders::EquirectangularToCubemap->setInt("equirectangularMap", 0);
	Shaders::EquirectangularToCubemap->setMat4("projection", captureProject);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); 
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for( int i = 0; i < 6; i++ )
	{
		Shaders::EquirectangularToCubemap->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderBakingCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);
	glDeleteTextures(1, &hdrTexture);

	textureID = cubemap;

}

CubemapTexture::~CubemapTexture()
{
	glDeleteTextures(1, &textureID);
}

void CubemapTexture::bind(unsigned int slot) const
{
	//Activate the texture slot
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); 
}
void CubemapTexture::unbind() const
{
	glActiveTexture(0);
}


