#pragma once
#include <string>
#include <SDL.h>
class Window 
{
public:
	Window( int width, int height, const std::string& title );
	
	~Window();

	bool init(); //Creates SDL Window and OpenGL context
		
	void setViewport( int width, int height ); //Just updates viewport dimensions. does not set the OpenGL viewport.

	void swapBuffers();

	SDL_Window* getWindow(){ return window; }

	SDL_GLContext getContext(){ return glContext; }

	int getWidth(){ return width; }

	int getHeight(){ return height; }

	void setVSync(bool enabled);

private:

	SDL_Window* window = nullptr;
	
	SDL_GLContext glContext = nullptr;
	
	int width, height;
	
	std::string title;
};