
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


Engine::Engine() :
	window(1280, 720, "Game Engine Sandrito"),
	engineCamera(glm::vec3(0.0f, 0.0f, 3.0f))
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

void Engine::drawUI(EntityManager& em, int frameRate)
{
	int numEntities = em.entities.size();

	//Draw the main Window
	//--------------------
	ImGui::Begin("Scene");
	ImGui::Text("FPS: %d", frameRate);
	ImGui::Text("Press F to start the game!");
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

void Engine::run( Game& game )
{
	int frameCount = 0;
	int frameRate = 0;

	//Render loop
	//-----------
	SDL_Event e;
	fpsTimer.start();
	isRunning = true;
	while (isRunning)
	{
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
		if( input.isKeyPressed(SDL_SCANCODE_ESCAPE) )
		{
			isPaused = !isPaused;
		}
		//Stop the loop if quit was requested
		//-----------------------------------
		if( input.isQuitRequested() )
		{ 
			isRunning = false; 
		}

		
		glm::mat4 viewMat;
		glm::mat4 projectMat;
		glm::vec3 viewPos;
		if( currentState == EngineState::Editor)
		{
			if(isPaused)
			{
				SDL_SetRelativeMouseMode(SDL_FALSE); //Cursor appears and is freed
			}
			else
			{
				SDL_SetRelativeMouseMode(SDL_TRUE);  //Cursor disappears and locks
				float xOffset = (float)input.getMouseXRel();
				float yOffset = (float)input.getMouseYRel();
				engineCamera.processMouseMovement(xOffset, -yOffset); //Invert Y for OpenGL
			}

			if( input.isKeyDown(SDL_SCANCODE_W) ) { engineCamera.processKeyboard(CameraMovement::FORWARD, dt); }
			if( input.isKeyDown(SDL_SCANCODE_A) ) { engineCamera.processKeyboard(CameraMovement::LEFT, dt); }
			if( input.isKeyDown(SDL_SCANCODE_S) ) { engineCamera.processKeyboard(CameraMovement::BACKWARD, dt); }
			if( input.isKeyDown(SDL_SCANCODE_D) ) { engineCamera.processKeyboard(CameraMovement::RIGHT, dt); }

			viewMat = engineCamera.getViewMatrix();
			projectMat = engineCamera.getProjectionMatrix(window.getWidth(), window.getHeight());
			viewPos = engineCamera.Position;

			if( input.isKeyPressed(SDL_SCANCODE_F) )
			{
				currentState = EngineState::Play;
			}
		}
		else if( currentState == EngineState::Play )
		{
			if (isPaused)
			{
				SDL_SetRelativeMouseMode(SDL_FALSE); //Cursor appears and is freed
			}

			if (input.isKeyPressed(SDL_SCANCODE_F))
			{
				currentState = EngineState::Editor;
			}
			
			game.update( dt );
			
			EntityManager& em = game.scene.getEntityManager();
			bool gameCameraFound = false;
			for( Entity e : em.entities )
			{
				CameraComponent* camComp = em.getCameraComponent(e);
				TransformComponent* transComp = em.getTransformComponent(e);
				if( camComp && transComp )
				{
					gameCameraFound = true;
					viewMat = glm::lookAt( transComp->position, transComp->position + transComp->Forward(), transComp->Up());
					projectMat = glm::perspective(glm::radians(camComp->Fov), (float)window.getWidth() / window.getHeight(), 
						camComp->NearPlane, camComp->FarPlane);
					viewPos = transComp->position;
				}
			}
			if( !gameCameraFound )
			{
				viewMat = engineCamera.getViewMatrix();
				projectMat = engineCamera.getProjectionMatrix(window.getWidth(), window.getHeight());
				viewPos = engineCamera.Position;
			}
		}

		//Render the scene with the interface if the game is running and without otherwise.	
		//-----------------------------------------------------------------------------------
		renderer.beginFrame(window.getWidth(), window.getHeight());
		renderer.setCamera( viewMat, projectMat, viewPos );
		
		//draw the game scene
		game.scene.update(renderer);

		if( currentState == EngineState::Editor )
		{
			//Draw the GUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
			drawUI(game.scene.getEntityManager(), frameRate);
		}

		//Swap the buffers
		window.swapBuffers();

		//Update the input
		//----------------
		input.update();

		//Cap the frame rate
		//------------------
		if( isFpsCapped )
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