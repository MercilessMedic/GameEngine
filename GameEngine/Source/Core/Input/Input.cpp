#include "Input.h"
#include <glad/glad.h>
#include <stdio.h>
#include <cstring>

Input::Input():
	mouseX(0),mouseY(0),quitRequested(false), windowResized(false), 
	windowWidth(0), windowHeight(0)
{	
	//Initialize key states to false
	std::memset( currentKeys, 0, sizeof(currentKeys) );
	std::memset( previousKeys, 0, sizeof(previousKeys) );

	//Initialize mouse button states to false;
	std::memset( currentMouseButtons, 0, sizeof(currentMouseButtons) );
	std::memset( previousMouseButtons, 0, sizeof(previousMouseButtons) );
}

void Input::processEvent(const SDL_Event& e)
{
	switch( e.type )
	{
	case SDL_KEYDOWN:
		currentKeys[e.key.keysym.scancode] = true; break;
	case SDL_KEYUP:
		currentKeys[e.key.keysym.scancode] = false; break;
	case SDL_MOUSEMOTION:
		mouseX = e.motion.x;       
		mouseY = e.motion.y;       
		mouseXRel += e.motion.xrel; 
		mouseYRel += e.motion.yrel; 
		break;
	case SDL_MOUSEBUTTONDOWN:
		currentMouseButtons[e.button.button] = true;
		break;
	case SDL_MOUSEBUTTONUP:
		currentMouseButtons[e.button.button] = false;
		break;
	case SDL_WINDOWEVENT:
		if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
		{
			windowWidth = e.window.data1;
			windowHeight = e.window.data2;
			windowResized = true;
		}
		break;
	case SDL_QUIT:
		quitRequested = true;
	
	}
}

void Input::update()
{	
	//Reset the offsets
	mouseXRel = 0; 
	mouseYRel = 0;

	//Update the previous keyboard state for the next frame
	std::memcpy( previousKeys, currentKeys, sizeof(currentKeys) );
	
	//Update the previous Mouse Button state for the next frame
	std::memcpy( previousMouseButtons, currentMouseButtons, sizeof( currentMouseButtons));
}

bool Input::isKeyDown(SDL_Scancode key) const
{
	return currentKeys[key];
}

//Checks wether the key was pressed only exactly at a specific frame
bool Input::isKeyPressed(SDL_Scancode key) const
{
	return currentKeys[key] && !previousKeys[key];
}

bool Input::isKeyReleased( SDL_Scancode key ) const
{
	return !currentKeys[key] && previousKeys[key];
}

bool Input::isMouseButtonDown(Uint8 button) const
{
	return currentMouseButtons[button];
}
//Checks wether the mouse button was pressed only exactly at a specific frame
bool Input::isMouseButtonPressed(Uint8 button) const
{
	return currentMouseButtons[button] && !previousMouseButtons[button];
}

bool Input::isMouseButtonReleased(Uint8 button) const
{
	return !currentMouseButtons[button] && previousMouseButtons[button];
}

