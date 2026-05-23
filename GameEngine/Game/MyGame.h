#include "Game.h"
#include "Core/Engine/Engine.h"
#include "Renderer/Shaders.h"

class MyGame : public Game
{
public:	
	glm::vec3 lightCubePosition = glm::vec3(1.0f, 1.0f, 1.0f);
	float radius = 3.0f;
	float speed = 1.0f;

	void init()
	{
		timer.start();

		Vertex cubeVertices[] =
		{
			// Position					   //Normal						 //TexCoords
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
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
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

		auto cubeVertexArray = std::make_shared<VertexArray>(cubeVertices, 36);
		auto lightCubeMesh = std::make_shared<Mesh>(cubeVertexArray);
		eLightCube = scene.createEntity();
		scene.addMesh(eLightCube, MeshComponent{ lightCubeMesh });

		auto knightModel = std::make_shared<Model>("Assets/generic_knight/scene.gltf");
		auto stoneModel = std::make_shared<Model>("Assets/stone/scene.gltf");
		Entity eStone = scene.createEntity();
		Entity eKnight = scene.createEntity();
		scene.addModel(eStone, ModelComponent{ stoneModel });
		scene.addModel(eKnight, ModelComponent{ knightModel });
		scene.addTag(eKnight, "TAMAZI");
		scene.addTag(eLightCube, "lightCube");
		printf("Initialized the game!");
	}

	void update()
	{
		//printf("Update the game!");
		float angle = timer.getTicks() / 1000.0f * speed;
		float x = radius * sin(angle);
		float z = radius * cos(angle);
		lightCubePosition.x = x;
		lightCubePosition.z = z;
		
		//update lightCube transform
		auto* transform = scene.getTransformComp(eLightCube);
		transform->position = lightCubePosition;

		Shaders::PBR->use();
		Shaders::PBR->setVec3("lightColors[0]", 150.0f, 150.0f, 150.0f);
		Shaders::PBR->setVec3("lightPositions[0]", lightCubePosition);
	}
private:
	Timer timer;
	Entity eLightCube;
};