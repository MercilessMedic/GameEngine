#include <stdio.h>
#include "Core/Engine/Engine.h"
#include "Game/MyGame.h"
#include <assimp/version.h>

int main(int argc, char* argv[])
{

	Engine myEngine;

	if( !myEngine.init() )
	{
		throw std::runtime_error("Failed to startup the engine!\n");
	}
	else
	{
		printf("Welcome to Game Engine Sandrito!\n");
	}

	MyGame game;
	game.setEngine( &myEngine );
	game.init();

	myEngine.run( game );
	myEngine.shutdown();

	return 0;
}
