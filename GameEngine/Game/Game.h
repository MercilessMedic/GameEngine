#pragma once
#include "Scene/Scene.h"
//#include "Core/Engine/Engine.h"

//Need this forward declaration because game uses the engine
//And the Engine uses the game which cleates circular dependency
class Engine;

class Game
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
	
	Engine* engine = nullptr;
	
	//Should make this vector of scenes later
	Scene scene;

	void setEngine(Engine* myEngine)
	{
		engine = myEngine;
	}
	
	
	
};