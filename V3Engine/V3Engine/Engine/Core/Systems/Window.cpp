#include "Window.h"
#include <SDL.h>
#include <glew.h>


Window::Window() : windowName("V3Engine") , windowWidth(800), windowHeight(600)
{
}

Window::Window(char * name, unsigned int width, unsigned int height) : windowName(name), windowWidth(width), windowHeight(height)
{
}

Window::~Window()
{
}

bool Window::Init()
{
	if (!InitSDL()) {
		return false;
	}

	windowParameters = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowParameters);
	//If failed to create window
	if (!window) {
		return false;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	//Creating a double buffer
	//Draw on back buffer display on front
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//V-sync
	SDL_GL_SetSwapInterval(1);

	//Set surface
	screenSurface = SDL_GetWindowSurface(window);

	//Remove mouse cursor and capture in window
	/*SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);*/

	return true;
}

void Window::Update()
{
	SDL_GL_SwapWindow(window);
}

void Window::Render()
{

}
bool Window::Shutdown()
{
	bool success = true;

	SDL_FreeSurface(screenSurface);
	screenSurface = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	if (window || screenSurface) {
		success = false;
	}

	CloseSDL();

	return success;
}

float Window::GetWidth()
{
	return GetScreenSurface()->w;
}

float Window::GetHeight()
{
	return GetScreenSurface()->h;
}

bool Window::InitSDL() {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		success = false;
	}

	return success;
}

bool Window::CloseSDL()
{
	SDL_Quit();

	return true;
}

void Window::Fullscreen() {
	if (!fullscreen) {
		windowParameters = windowParameters | SDL_WINDOW_FULLSCREEN_DESKTOP;
		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowParameters);
		fullscreen = true;
	}
	else {
		windowParameters = windowParameters ^ SDL_WINDOW_FULLSCREEN_DESKTOP;
		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowParameters);
		fullscreen = false;
	}

}
