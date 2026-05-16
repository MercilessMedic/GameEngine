#include <stdio.h>
#include "Core/Engine/Engine.h"
#include <assimp/version.h>

int main(int argc, char* argv[])
{

	Engine myEngine;
	
	if( !myEngine.init() )
	{
		printf("Failed to startup the engine!\n");
		return 1;
	}
	else
	{
		printf("Welcome to Game Engine Sandrito!\n");
	}

	myEngine.run();
	myEngine.shutdown();
	return 0;
}
