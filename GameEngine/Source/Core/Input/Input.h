#pragma once
#include <SDL.h>

class Input
{
public:

	static const int NUM_MOUSE_BUTTONS = 5;

	Input();

	void processEvent( const SDL_Event& e);
	void update();

	bool isKeyDown( SDL_Scancode key )         const;

	bool isKeyPressed( SDL_Scancode key )      const;
	
	bool isKeyReleased( SDL_Scancode key )     const;

	bool isMouseButtonDown( Uint8 button )	   const;

	bool isMouseButtonPressed( Uint8 button )  const;

	bool isMouseButtonReleased( Uint8 button ) const;

	bool isQuitRequested(){ return quitRequested; }

	bool wasWindowResized(){ return windowResized; }

	void getWindowSize(int& w, int& h) { w = windowWidth; h = windowHeight; }

	void clearResizedFlag() { windowResized = false; }

	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }
	int getMouseXRel() const { return mouseXRel; }
	int getMouseYRel() const { return mouseYRel; }

private:

	bool currentKeys[SDL_NUM_SCANCODES];
	bool previousKeys[SDL_NUM_SCANCODES];
	
	bool currentMouseButtons[NUM_MOUSE_BUTTONS];
	bool previousMouseButtons[NUM_MOUSE_BUTTONS];

	int mouseX, mouseY;
	//Represents relative movement of the mouse
	int mouseXRel, mouseYRel;
	int windowWidth,windowHeight;
	
	bool quitRequested;
	bool windowResized;
};