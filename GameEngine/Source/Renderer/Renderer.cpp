#include <glad/glad.h>
#include <stdio.h>
#include "Renderer.h"
#include "Scene/Scene.h"

void Renderer::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	//glEnable( GL_CULL_FACE );

	//Vertex buffer for skybox VertexArray
	Vertex cubeVertices[] =
	{
		{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.1f) }, 
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
	};

	skyboxVAO = std::make_shared<VertexArray>(cubeVertices, 36);
	printf("Renderer Initialized, OpenGL version: %s\n", glGetString(GL_VERSION));
}

void Renderer::beginFrame(int width, int height)
{
	glViewport(0, 0, width, height);
	clear();
}

void Renderer::setCamera( glm::mat4 viewMat, glm::mat4 projectMat, glm::vec3 viewPos ) 
{
	curCameraView = viewMat;
	curCameraProject = projectMat;
	curCameraViewPos = viewPos;
};

void Renderer::drawSkybox(std::shared_ptr<CubemapTexture> cubemap)
{
	
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE); 

	Shaders::Skybox->use();
	Shaders::Skybox->setMat4("projection", curCameraProject);

	glm::mat4 view = glm::mat4(glm::mat3(curCameraView));
	Shaders::Skybox->setMat4("view", view);

	cubemap->bind(0);
	Shaders::Skybox->setInt("environmentMap", 0);

	skyboxVAO->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	skyboxVAO->unbind();

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

}

void Renderer::draw(std::shared_ptr<VertexArray> vertexArray, Shader& shaderProgram)
{
	vertexArray->bind();
	if( activeShaderId != shaderProgram.getId() )
	{
		shaderProgram.use();
		activeShaderId = shaderProgram.getId();
	}

	if(vertexArray->isIndexed())
	{
		glDrawElements( GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr );
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->getVertexCount());
	}
	vertexArray->unbind();
}

void Renderer::draw(const Mesh& mesh, const glm::mat4& modelMatrix )
{
	
	auto& material = mesh.getMaterial();
	if( material == NULL )
	{
		cout << "[WARNING] [RENDERER] Trying to draw a mesh without a material!!\n";
		return;
	}
	
	Shader& shader = material->getShader();
	shader.use();
	//
	//if( activeShaderId != shader.getId() )
	//{
	//	shader.use();
	//	activeShaderId = shader.getId();
	//}
	
	glm::mat3 normalMatrix = glm::transpose( glm::inverse(glm::mat3(modelMatrix)) );
	
	material->bind();
	
	shader.setMat4("model", modelMatrix);
	shader.setMat4("view", curCameraView);
	shader.setMat4("projection", curCameraProject);
	shader.setMat3("normalMatrix", normalMatrix);
	shader.setVec3("viewPos", curCameraViewPos);

	draw(mesh.getVertexArray(), shader);
}

void Renderer::draw(const Model& model, const glm::mat4& modelMatrix)
{
	for (const auto& mesh : model.getMeshes())
	{		
		draw( mesh, modelMatrix );
	}
}

void Renderer::clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


	