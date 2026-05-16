
#include "Engine.h"

#include "Renderer/Shader.h"
#include "Renderer/Shaders.h"
#include "Renderer/PhongMaterial.h"
#include "Renderer/PBRMaterial.h"
#include "Renderer/UnlitMaterial.h"

#include <stb_image.h>
#include <imgui.h>
#include <ImGui/backends/imgui_impl_sdl2.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include "Scene/EntityManager.h"
#include "Scene/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene scene;

Engine::Engine() :
	window(1280, 720, "Game Engine Sandrito"),
	camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	isRunning = false;
	isPaused = false;
}

bool Engine::init()
{
	if (!window.init()) { return false; }

	renderer.init();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	isRunning = true;

	//Create the shaders
	Shaders::PBR = std::make_shared<Shader>("Source/Shaders/PBR.vert", "Source/Shaders/PBR.frag");
	Shaders::Phong = std::make_shared<Shader>("Source/Shaders/Phong.vert", "Source/Shaders/Phong.frag");
	Shaders::Unlit = std::make_shared<Shader>("Source/Shaders/Unlit.vert", "Source/Shaders/Unlit.frag");

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getContext());
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 1.3f;

	return true;
}

void Engine::drawUI(int frameRate)
{
	EntityManager& em = scene.getEntityManager();
	int numEntities = em.entities.size();

	//Draw the main Window
	//--------------------
	ImGui::Begin("Scene");
	ImGui::Text("FPS: %d", frameRate);
	for( Entity e : em.entities )
	{
		std::string eTag = em.getTagComponent(e)->tag;
		bool isSelected = (e == editorState.currentEntity);
		if(  isSelected )
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.2f, 0.0f, 1.0f));
		}

		if( ImGui::Button(eTag.c_str()) )
		{
			editorState.currentEntity = e;
		};

		if( isSelected )
		{
			ImGui::PopStyleColor(3);
		}

	}

	if(isFpsCapped)
	{
		char buff[] = "Uncap fps";
		
		if (ImGui::Button(buff))
		{
			isFpsCapped = !isFpsCapped;
			std::cout << "Changes Frame cap!\n";
		}
	}
	else
	{
		char buff[] = "cap fps";
		if (ImGui::Button(buff))
		{
			isFpsCapped = !isFpsCapped;
			std::cout << "Changes Frame cap!\n";
		}
	}
	ImGui::End();

	//Draw the inspector
	//---------------------
	ImGui::Begin("Inspector");
	if( editorState.currentEntity != INVALID_ENTITY )
	{
		TransformComponent* transform = em.getTransformComponent(editorState.currentEntity);
		
		if( transform )
		{
			ImVec4 Red = { 1.0, 0.0, 0.0, 1.0 };
			ImVec4 Green = { 0.0, 1.0, 0.0, 1.0 };
			ImVec4 Blue = { 0.0, 0.0, 1.0, 1.0 };
			//Position
			//--------
			glm::vec3& position = transform->position;
			ImGui::Text("Position");
			ImGui::Separator();
			ImGui::TextColored(Red, "x");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##posX", &position.x, 0.05f, -1000.0f, 1000.0f);

			ImGui::SameLine();
			ImGui::TextColored(Green, "y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##posY", &position.y, 0.05f, -1000.0f, 1000.0f);

			ImGui::SameLine();
			ImGui::TextColored(Blue, "Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##posZ", &position.z, 0.05f, -1000.0f, 1000.0f);

			//Rotation
			//--------
			glm::vec3& rotation = transform->rotation;
			ImGui::Text("Rotation");
			ImGui::Separator();
			ImGui::TextColored(Red, "x");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##rotX", &rotation.x, 0.5f, -360.0f, 360.0f);

			ImGui::SameLine();
			ImGui::TextColored(Green, "y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##rotY", &rotation.y, 0.5f, -360.0f, 360.0f);

			ImGui::SameLine();
			ImGui::TextColored(Blue, "Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##rotZ", &rotation.z, 0.5f, -360.0f, 360.0f);
			
			//scale
			//--------
			glm::vec3& scale = transform->scale;
			ImGui::Text("Scale");
			ImGui::Separator();
			ImGui::TextColored(Red, "x");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##scalX", &scale.x, 0.2f, 0.0f, 1000.0f);

			ImGui::SameLine();
			ImGui::TextColored(Green, "y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##scalY", &scale.y, 0.2f, 0.0f, 1000.0f);

			ImGui::SameLine();
			ImGui::TextColored(Blue, "Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			ImGui::DragFloat("##scalZ", &scale.z, 0.2f, 0.0f, 1000.0f);
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
}

void Engine::capFPS( Uint32 frameStart )
{
	Uint32 frameTime = SDL_GetTicks64() - frameStart;
	if (frameTime < FRAME_DELAY)
	{
		SDL_Delay(FRAME_DELAY - frameTime);
	}
}

void Engine::run()
{
	int frameCount = 0;
	int frameRate = 0;

	//Cube setup
	//---------
	Vertex cubeVertices[] =
	{
		// Position                         // Normal                // TexCoords
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

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
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) }
	};

	auto cubeVertexArray = std::make_shared<VertexArray>(cubeVertices, 36);
	auto lightCubeMesh = std::make_shared<Mesh>(cubeVertexArray);
	Entity eLightCube = scene.createEntity();
	scene.addMesh(eLightCube, MeshComponent{ lightCubeMesh });
	scene.addTransform(eLightCube, TransformComponent());

	
	auto knightModel = std::make_shared<Model>("Assets/generic_knight/scene.gltf");
	auto stoneModel = std::make_shared<Model>("Assets/stone/scene.gltf");
	Entity eStone = scene.createEntity();
	Entity eKnight = scene.createEntity();
	scene.addModel(eStone, ModelComponent{ stoneModel });
	scene.addModel(eKnight, ModelComponent{ knightModel });
	//scene.addTag(eStone, "Stone");
	//scene.addTag(eKnight, "Knight");
	//Render loop
	//-----------
	SDL_Event e;
	Timer timer;
	timer.start();
	fpsTimer.start();
	isRunning = true;
	glm::vec3 lightCubePosition = glm::vec3(1.0f, 1.0f, 1.0f);

	while (isRunning)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		float radius = 3.0f;
		float speed = 1.0f;
		float angle = timer.getTicks() / 1000.0f * speed;
		float x = radius * sin(angle);
		float z = radius * cos(angle);
		lightCubePosition.x = x;
		lightCubePosition.z = z;

		Uint32 frameStart = SDL_GetTicks64();
		float dt = fpsTimer.getDeltaSeconds();

		//Process all the events
		//---------------------
		while (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			input.processEvent(e);
		}

		//Check if window was resized
		//---------------------------
		if (input.wasWindowResized())
		{
			int w, h;
			input.getWindowSize(w, h);
			window.setViewport(w, h);
			input.clearResizedFlag();
		}

		//Lock/unlock mouse Cursor
		//------------------------
		if (input.isKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			isPaused = !isPaused;
		}
		if (isPaused)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE); //Cursor appears and is freed
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);  //Cursor disappears and locks
			float xOffset = (float)input.getMouseXRel();
			float yOffset = (float)input.getMouseYRel();
			camera.processMouseMovement(xOffset, -yOffset); //Invert Y for OpenGL
		}

		//Handle camera movement
		//----------------------
		if (input.isQuitRequested()) { isRunning = false; }
		if (input.isKeyDown(SDL_SCANCODE_W)) { camera.processKeyboard(CameraMovement::FORWARD, dt); }
		if (input.isKeyDown(SDL_SCANCODE_A)) { camera.processKeyboard(CameraMovement::LEFT, dt); }
		if (input.isKeyDown(SDL_SCANCODE_S)) { camera.processKeyboard(CameraMovement::BACKWARD, dt); }
		if (input.isKeyDown(SDL_SCANCODE_D)) { camera.processKeyboard(CameraMovement::RIGHT, dt); }

		//Mouse button test
		//-----------------
		//if (input.isMouseButtonPressed(SDL_BUTTON_RIGHT)) { printf("Right click was pressed!\n"); }
		//if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) { printf("Left click was pressed!\n"); }

		//Render the scene	
		//------------------
		renderer.beginFrame(window.getWidth(), window.getHeight());
		renderer.setCamera(camera, window.getWidth(), window.getHeight());

		Shaders::PBR->use();
		Shaders::PBR->setVec3("lightColors[0]", 150.0f, 150.0f, 150.0f);
		Shaders::PBR->setVec3("lightPositions[0]", lightCubePosition);

		auto* transform = scene.getTransformComp(eLightCube);
		transform->position = lightCubePosition;

		//Update and draw the scene
		//---------------------------
		scene.update(renderer);

		//Draw the GUI
		//------------
		drawUI(frameRate);

		//Swap the buffers
		//----------------
		window.swapBuffers();

		//Update the input
		//----------------
		input.update();

		//Cap the frame rate
		//------------------
		if (isFpsCapped)
		{
			capFPS( frameStart );
		}

		//Calculate the frame rate
		//------------------------
		frameCount++;
		if (fpsTimer.getTicks() >= 1000) //One second has passed 
		{

			//std::cout << "FPS:" << frameCount << "\n";
			frameRate = frameCount;
			frameCount = 0;
			fpsTimer.start();
		}
	}
}

void Engine::shutdown()
{
	SDL_DestroyWindow(window.getWindow());
	SDL_GL_DeleteContext(window.getContext());
	SDL_Quit();
}