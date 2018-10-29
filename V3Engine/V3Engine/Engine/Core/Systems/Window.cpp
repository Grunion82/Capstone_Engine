#include "Window.h"
#include <SDL.h>
#include <glew.h>


Window::Window() : windowName("V3Engine") , windowWidth(800), windowHeight(600)
{
}

Window::Window(const char * name, unsigned int width, unsigned int height) : windowName(name), windowWidth(width), windowHeight(height)
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
	SDL_GL_SetSwapInterval(vsync);

	//Set surface
	screenSurface = SDL_GetWindowSurface(window);

	//Remove mouse cursor and capture in window
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return true;
}

void Window::Update()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event)
			{
			case(SDL_WINDOWEVENT_SHOWN):
			case(SDL_WINDOWEVENT_EXPOSED)://Window is exposed and needs to be redrawn?
			case(SDL_WINDOWEVENT_FOCUS_GAINED):
			case(SDL_WINDOWEVENT_ENTER)://Window has gained mouse focus
				SDL_ShowCursor(SDL_DISABLE);
				SDL_SetWindowGrab(window, SDL_TRUE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				break;
			case(SDL_WINDOWEVENT_HIDDEN):
			case(SDL_WINDOWEVENT_FOCUS_LOST):
			case(SDL_WINDOWEVENT_LEAVE)://Window has lost mouse focus
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetWindowGrab(window, SDL_FALSE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
				break;
			case(SDL_WINDOWEVENT_RESIZED):
			case(SDL_WINDOWEVENT_SIZE_CHANGED)://The window size has changed, either as a result of an API call or through the system or user changing the window size
				screenSurface = SDL_GetWindowSurface(window);
				break;
			/*
			case(SDL_WINDOWEVENT_MINIMIZED):
				break;
			case(SDL_WINDOWEVENT_MAXIMIZED):
				break;
			case(SDL_WINDOWEVENT_CLOSE)://Window manager requests window to be closed
				break;
			case(SDL_WINDOWEVENT_TAKE_FOCUS)://Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore)
				break;
			case(SDL_WINDOWEVENT_HIT_TEST)://Window had a hit test that wasn't SDL_HITTEST_NORMAL
				break;
			case(SDL_WINDOWEVENT_RESTORED)://Window has been set back to normal position and size
			*/

			default:
				break;
			}
		}
	}

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

int Window::GetWidth()
{
	return GetScreenSurface()->w;
}

int Window::GetHeight()
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
		windowParameters |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		fullscreen = true;
	}
	else {
		windowParameters ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
		fullscreen = false;
	}
	SDL_SetWindowFullscreen(window, windowParameters);
}

void Window::Borderless()
{
	SDL_bool b;
	if (!borderless) {
		windowParameters |= SDL_WINDOW_BORDERLESS;
		borderless = true;
		b = SDL_TRUE;
		
	}
	else {
		windowParameters ^= SDL_WINDOW_BORDERLESS;
		borderless = false;
		b = SDL_FALSE;
	}
	SDL_SetWindowBordered(window, b);
}

void Window::VSync()
{
	vsync = vsync ? false : true;
	SDL_GL_SetSwapInterval(vsync);
}

