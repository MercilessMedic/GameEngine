#include "Game.h"
#include "Core/Engine/Engine.h"
#include "Renderer/Shaders.h"
#include "Renderer/UnlitMaterial.h"
#include "Renderer/PBRMaterial.h"

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
		std::string skyboxPath1 = "Assets/skyboxes/sunset.hdr";
		std::string skyboxPath2 = "Assets/skyboxes/night.hdr";
		std::string skyboxPath3 = "Assets/skyboxes/misty.hdr";
		std::string skyboxPath4 = "Assets/skyboxes/Puresky.hdr";
		cubemap1 = std::make_shared<CubemapTexture>(skyboxPath1);
		cubemap2 = std::make_shared<CubemapTexture>(skyboxPath2);
		cubemap3 = std::make_shared<CubemapTexture>(skyboxPath3);
		cubemap4 = std::make_shared<CubemapTexture>(skyboxPath4);
		
		scene.setSkyboxTex( nullptr );

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
			{
				glm::vec3(-0.5f, 0.0f, -0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
},
			{
				glm::vec3(0.5f, 0.0f, -0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(1.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			},
			{
				glm::vec3(0.5f, 0.0f,  0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(1.0f, 1.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			},

			{
				glm::vec3(0.5f, 0.0f,  0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(1.0f, 1.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			},
			{
				glm::vec3(-0.5f, 0.0f,  0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(0.0f, 1.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			},
			{
				glm::vec3(-0.5f, 0.0f, -0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec2(0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			}
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
		auto lightCubeMaterial = scene.getEntityManager().getMeshComponent(eLightCube1)->mesh->getMaterialAs<UnlitMaterial>();
		lightCubeMaterial->color = color;

		eLightCube2 = scene.createEntity();
		scene.addPointLight(eLightCube2, PointlightComponent{ color });
		scene.addMesh(eLightCube2, MeshComponent{ lightCubeMesh });

		eLightCube3 = scene.createEntity();
		scene.addPointLight(eLightCube3, PointlightComponent{ color });
		scene.addMesh(eLightCube3, MeshComponent{ lightCubeMesh });

		//Setup the plane
		ePlane = scene.createEntity();
		scene.addMesh(ePlane, MeshComponent{ planeMesh });
		auto albedo = std::make_shared<Texture>(
			"Assets/Materials/forest_ground/textures/forest_ground_diff.jpg",
			TextureType::ALBEDO
		);

		auto normal = std::make_shared<Texture>(
			"Assets/Materials/forest_ground/textures/forest_ground_nor.jpg",
			TextureType::NORMAL
		);

		auto roughness = std::make_shared<Texture>(
			"Assets/Materials/forest_ground/textures/forest_ground_rough.jpg",
			TextureType::ROUGHNESS
		);
		auto planeMaterial = std::make_shared<PBRMaterial>();
		planeMaterial->albedoMap = albedo;
		planeMaterial->normalMap = normal;
		planeMaterial->roughnessMap = roughness;
		scene.getEntityManager().getMeshComponent( ePlane )->mesh->setMaterial(planeMaterial);
		auto planeTrans = scene.getEntityManager().getTransformComponent(ePlane);
		planeTrans->position.y = -1.0f;
		planeTrans->scale = glm::vec3(100.0f);

		Player = scene.createEntity();
		auto sevarogModel = std::make_shared<Model>("Assets/sevarog/Sevarog.gltf");
		scene.addModel(Player, ModelComponent{ sevarogModel });

		Entity eStone = scene.createEntity();
		auto stoneModel = std::make_shared<Model>("Assets/stone/scene.gltf");
		scene.addModel(eStone, ModelComponent{ stoneModel });

		
		eCamera = scene.createEntity();
		
		scene.addCamera( eCamera, CameraComponent{});
		TransformComponent* cameraTransform = scene.getTransformComp( eCamera );
		CameraComponent* cameraComp = scene.getCameraComp(eCamera);
		cameraTransform->position = cameraComp->Offset;
		
		glm::vec3 rockPositions[] =
		{ 
			glm::vec3(0.0f, -0.7f, -2.6f),
			glm::vec3(4.0f, -0.7f, 0.0f),
			glm::vec3(0.0f,  -0.7f, 5.0f)
		};
		auto rocksModel = std::make_shared<Model>("Assets/rockSet1/rockSet1.gltf");
		for(int i = 0; i < 3; i++)
		{
			Entity eRocks = scene.createEntity();
			TransformComponent* rocksTrans = scene.getTransformComp(eRocks);
			rocksTrans->position = rockPositions[i];
			scene.addModel(eRocks, ModelComponent{ rocksModel });
			scene.addTag(eRocks, "rocks" + std::to_string(i));
		}

		glm::vec3 knightPositions[] =
		{
			glm::vec3(1.0f, 0.125f, -2.6f),
			glm::vec3(4.8f, 0.125f, -2.6f),
			glm::vec3(-2.7f, 0.125f, -2.6f)
		};
		auto knightModel = std::make_shared<Model>("Assets/generic_knight/scene.gltf");
		for (int i = 0; i < 3; i++)
		{
			Entity eKnight = scene.createEntity();
			TransformComponent* knightTrans = scene.getTransformComp(eKnight);
			knightTrans->position = knightPositions[i];
			scene.addModel(eKnight, ModelComponent{ knightModel });
			scene.addTag(eKnight, "knight" + std::to_string(i));
		}


		scene.addTag(Player, "PlayerKnight");
		scene.addTag(eLightCube1, "lightCube1");
		scene.addTag(eLightCube2, "lightCube2");
		scene.addTag(eLightCube3, "lightCube3");
		scene.addTag(ePlane, "Plane");
		scene.addTag(eCamera, "camera");

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


		//Chnage the skybox
		//-----------------
		if(input.isKeyPressed(SDL_SCANCODE_0))
		{
			scene.setSkyboxTex(nullptr);
		}
		if(input.isKeyPressed(SDL_SCANCODE_1))
		{
			scene.setSkyboxTex(cubemap1);
		}
		if (input.isKeyPressed(SDL_SCANCODE_2))
		{
			scene.setSkyboxTex(cubemap2);
		}
		if (input.isKeyPressed(SDL_SCANCODE_3))
		{
			printf("Pressed 3");
			scene.setSkyboxTex(cubemap3);
		}
		if (input.isKeyPressed(SDL_SCANCODE_4))
		{
			printf("Pressed 4");
			scene.setSkyboxTex(cubemap4);
		}

		auto* playerTrans = scene.getTransformComp(Player);
		auto* camTrans = scene.getTransformComp(eCamera);
		if (playerTrans && camTrans)
		{
			//Define the target point
			glm::vec3 targetLookAt = playerTrans->position + glm::vec3(0.0f, 2.0f, 0.0f);

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
	Entity eLightCube3;
	Entity ePlane;
	Entity Player;
	Entity eCamera;
	std::shared_ptr<CubemapTexture> cubemap1;
	std::shared_ptr<CubemapTexture> cubemap2;
	std::shared_ptr<CubemapTexture> cubemap3;
	std::shared_ptr<CubemapTexture> cubemap4;
};