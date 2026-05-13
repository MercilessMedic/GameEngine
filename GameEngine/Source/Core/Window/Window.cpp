#include "Window.h"
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

Window::Window( int w, int h, const std::string& t )
{
	width = w;
	height = h;
	title = t;
}

Window::~Window()
{
	if ( glContext ) SDL_GL_DeleteContext(glContext);
	if ( window ) SDL_DestroyWindow(window);
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf(" Failed to initialize SDL\n");
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
		//Create the window
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 
									SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
		if(window == NULL)
		{
			printf("Failed to create an SDL window!\n");
			return false;
		}

		//Create the OpenGL context
		glContext = SDL_GL_CreateContext(window);
		if( !glContext ) 
		{
			printf("Failed to create OpenGL context!\n");
			return false;
		}
		SDL_GL_MakeCurrent(window, glContext);

		//Load OpenGL functions
		if( !gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) )
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}
	}
	setViewport( width, height );
	return true;
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void Window::setViewport( int w, int h ) 
{
	width = w;
	height = h;
}

void Window::setVSync(bool enabled) 
{
	SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}