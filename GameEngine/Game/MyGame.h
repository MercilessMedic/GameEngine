#include "Game.h"
#include "Core/Engine/Engine.h"
#include "Renderer/Shaders.h"
#include "Renderer/UnlitMaterial.h"

enum class playerMovementDir
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class MyGame : public Game
{
public:	
	
	//Lightcube attributes
	glm::vec3 lightCubePosition = glm::vec3(1.0f, 1.0f, 1.0f);
	float radius = 3.0f;
	float cubeSpeed = 1.0f;

	//Camera attributes
	float camYaw = -90.0f;
	float camPitch = 0.0f;
	float mouseSensitivity = 0.1f;
	float cameraDistance = 6.0f;

	//Player attributes
	float movementSpeed = 5.0f;
	float rotationSpeed = 120.0f;
	int health = 100;

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

		Vertex planeVertices[] =
		{
			// First triangle
			{ glm::vec3(-0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3(0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

			// Second triangle
			{ glm::vec3(0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(-0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
			{ glm::vec3(-0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		};

		auto cubeVertexArray = std::make_shared<VertexArray>(cubeVertices, 36);
		auto planeVertexArray = std::make_shared<VertexArray>(planeVertices, 6);
		auto lightCubeMesh = std::make_shared<Mesh>(cubeVertexArray);
		auto planeMesh = std::make_shared<Mesh>(planeVertexArray);
		
		//Light cube 1
		glm::vec3 color(1.0);
		eLightCube1 = scene.createEntity();
		scene.addPointLight(eLightCube1, PointlightComponent{ color });
		scene.addMesh(eLightCube1, MeshComponent{ lightCubeMesh });

		eLightCube2 = scene.createEntity();
		scene.addPointLight(eLightCube2, PointlightComponent{ color });
		scene.addMesh(eLightCube2, MeshComponent{ lightCubeMesh });

		Entity eLightCube3 = scene.createEntity();
		scene.addPointLight(eLightCube3, PointlightComponent{ color });
		scene.addMesh(eLightCube3, MeshComponent{ lightCubeMesh });


		ePlane = scene.createEntity();
		scene.addMesh(ePlane, MeshComponent{ planeMesh });


		
		auto planeMaterial = scene.getEntityManager().getMeshComponent( ePlane )->mesh->getMaterialAs<UnlitMaterial>();
		planeMaterial->color = glm::vec3{ 0.0, 0.5, 0.0};
		
		auto knightModel = std::make_shared<Model>("Assets/generic_knight/scene.gltf");
		auto stoneModel = std::make_shared<Model>("Assets/stone/scene.gltf");
		
		Entity eStone = scene.createEntity();
		Player = scene.createEntity();
		eCamera = scene.createEntity();
		
		scene.addCamera( eCamera, CameraComponent{});
		TransformComponent* cameraTransform = scene.getTransformComp( eCamera );
		CameraComponent* cameraComp = scene.getCameraComp(eCamera);
		cameraTransform->position = cameraComp->Offset;
		
		scene.addModel(eStone, ModelComponent{ stoneModel });
		scene.addModel(Player, ModelComponent{ knightModel });
		scene.addTag(Player, "PlayerKnight");
		scene.addTag(eLightCube1, "lightCube1");
		scene.addTag(eLightCube2, "lightCube2");
		scene.addTag(ePlane, "Plane");
		scene.addTag(eCamera, "camera");
		scene.addTag(eStone, "stone");
		//Shaders::PBR->use();
		//Shaders::PBR->setVec3("lightColors[0]", 150.0f, 150.0f, 150.0f);
		//Shaders::PBR->setVec3("lightPositions[0]", lightCubePosition);
		printf("Initialized the game!");
	}

	void update( float dt )
	{
		Input input = engine->getInput();

		float xOffset = input.getMouseXRel();
		float yOffset = input.getMouseYRel();
		processMouseMovement(xOffset, -yOffset);

		float angle = timer.getTicks() / 1000.0f * cubeSpeed;
		float x = radius * sin(angle);
		float z = radius * cos(angle);
		lightCubePosition.x = x;
		lightCubePosition.z = z;

		//update lightCube transform
		auto* transform = scene.getTransformComp(eLightCube1);
		transform->position = lightCubePosition;

		//Player movement
		//---------------
		if (input.isKeyDown(SDL_SCANCODE_W)) { movePlayer(playerMovementDir::FORWARD, dt); }
		if (input.isKeyDown(SDL_SCANCODE_S)) { movePlayer(playerMovementDir::BACKWARD, dt); }
		if (input.isKeyDown(SDL_SCANCODE_A)) { movePlayer(playerMovementDir::LEFT, dt); }
		if (input.isKeyDown(SDL_SCANCODE_D) ){ movePlayer(playerMovementDir::RIGHT, dt); }

		auto* playerTrans = scene.getTransformComp(Player);
		auto* camTrans = scene.getTransformComp(eCamera);
		if (playerTrans && camTrans)
		{
			//Define the target point
			glm::vec3 targetLookAt = playerTrans->position + glm::vec3(0.0f, 0.0f, 0.0f);

			//Pull the camera backwards
			camTrans->position = targetLookAt - (camTrans->Forward() * cameraDistance);
		}

		Shaders::PBR->use();
		Shaders::PBR->setVec3("lightColors[0]", 150.0f, 150.0f, 150.0f);
		Shaders::PBR->setVec3("lightPositions[0]", lightCubePosition);
	}

	void movePlayer(playerMovementDir dir, float dt )
	{
		EntityManager& em = scene.getEntityManager();
		TransformComponent* camTrans = em.getTransformComponent(eCamera);
		TransformComponent* playerTrans = em.getTransformComponent(Player);
		glm::vec3& position = playerTrans->position;
		glm::vec3& rotation = playerTrans->rotation;


		//Flatten Y components so that player does not fly up in the air
		glm::vec3 camForward = camTrans->Forward();
		glm::vec3 camRight = camTrans->Right();
		camRight.y = 0.0f;
		camRight = glm::normalize(camRight);
		camForward.y = 0.0f;
		camForward = glm::normalize(camForward);
		
		glm::vec3 moveDir(0.0f);

		switch( dir )
		{
		case playerMovementDir::FORWARD:
			moveDir = camForward;
			break;
		case playerMovementDir::BACKWARD:
			moveDir = -camForward;
			break;
		case playerMovementDir::LEFT:
			moveDir = -camRight;
			break;
		case playerMovementDir::RIGHT:
			moveDir = camRight;
			break;
		}
		
		position += moveDir * movementSpeed * dt;

		//Rotate the model based on movemend direction
		if (glm::length(moveDir) > 0.001f)
		{
			float yaw = glm::degrees(glm::atan(moveDir.x, moveDir.z));
			rotation.y = yaw;
		}

	}

	void processMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		camYaw += xoffset;
		camPitch += yoffset;

		if (camPitch> 89.0f) { camPitch = 89.0f; }
		if (camPitch < -89.0f) { camPitch = -89.0f; }

		rotateCamera();
	}

	void rotateCamera()
	{
		TransformComponent* transform = scene.getEntityManager().getTransformComponent(eCamera);
		if( transform )
		{
			transform->rotation.y = glm::radians(camYaw);   
			transform->rotation.x = glm::radians(camPitch);
		}
	}
private:
	Timer timer;
	Entity eLightCube1;
	Entity eLightCube2;
	Entity ePlane;
	Entity Player;
	Entity eCamera;
};