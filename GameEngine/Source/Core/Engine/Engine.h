#pragma once

#include <SDL.h>
#include <iostream>
#include <glad/glad.h>

#include "Core/Window/Window.h"
#include "Core/Input/Input.h"
#include "Core/Timer/Timer.h"
#include "Core/Camera/Camera.h"

#include "Game/Game.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shaders.h"

#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/TransformSystem.h"

#include "EditorState.h"

enum class EngineState
{
	Editor,
	Play
};

class Engine
{
public:
	
	Engine();

	bool init();
	
	void drawUI( EntityManager& em, int frameRate );

	void capFPS( Uint32 frameStart );

	void run( Game& game );

	void shutdown();

	Window& getWindow() { return window; }

	
private:
	Window window;
	Renderer renderer;
	Input input;
	Timer fpsTimer;
	Camera camera;
	EditorState editorState;
	const int TARGET_FPS = 30;
	const int FRAME_DELAY = 1000 / TARGET_FPS;

	bool isRunning;
	bool isPaused;
	bool isFpsCapped;
};