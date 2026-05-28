#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Core/Camera/Camera.h"

class Renderer
{
public:
	
	void init();
		
	//Guarantees a clean slate for every frame. It sets the OpenGl viewport and calls clear
	void beginFrame( int width, int height );

	//Sets camera matrices and position in the renderer
	void setCamera( glm::mat4 viewMat, glm::mat4 projectMat, glm::vec3 viewPos );

	//Draw function for Vertex Array
	void draw(std::shared_ptr<VertexArray> vertexArray, Shader& shader);

	//Draw a mesh with it's material
	void draw( const Mesh& mesh, const glm::mat4& modelMatrix);
	
	//Draw meshes of the model with their materials
	void draw( const Model& model, const glm::mat4& modelMatrix);
private:
	void clear();
	//Currently active shader id
	unsigned int activeShaderId;

	glm::mat4 curCameraView;
	glm::mat4 curCameraProject;
	glm::vec3 curCameraViewPos;

};