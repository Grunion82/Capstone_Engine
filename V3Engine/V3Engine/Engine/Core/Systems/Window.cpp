#include "Window.h"
#include <SDL.h>
#include <glew.h>

#include "Debug.h"

Window::Window(const char* name) : windowName(name), windowWidth(0), windowHeight(0)  {

}

Window::Window(unsigned int width, unsigned int height) : windowName("Window"), windowWidth(width)
{
	
}

Window::Window(const char * name, unsigned int width, unsigned int height) : windowName(name), windowWidth(width), windowHeight(height)
{
}

bool Window::Init()
{
	if (!InitSDL()) {
		return false;
	}


	SDL_DisplayMode mode;
	//int display_mode_count = SDL_GetNumVideoDisplays();//For current display
	int display_mode_count = SDL_GetNumDisplayModes(0);//Get the number of display modes for display 0(I assume default)
	windowResolutions.reserve(display_mode_count);

	for (int i = 0; i < display_mode_count; ++i) {
		if (SDL_GetDisplayMode(0, i, &mode) != 0) {
			SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
			std::string message = "SDL_GetDisplayMode failed; ";;
			message += SDL_GetError();
			Debug::SetSeverity(MessageType::TYPE_WARNING);
			Debug::Warning(message, __FILE__, __LINE__);
		}
		//Uint32 f = mode.format;
		//SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i\t %i", i, SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h, mode.refresh_rate);
		windowResolutions.push_back(mode);
	}

	windowParameters = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	if (windowWidth != 0 && windowHeight != 0) {
		for (unsigned int i = 0; i < windowResolutions.size(); i++) {
			if (windowResolutions[i].w == windowWidth && windowResolutions[i].h == windowHeight) {
				currentDisplay = windowResolutions[i];
			}
		}

		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowParameters);
	}
	else {
		int displays = SDL_GetNumVideoDisplays();
		SDL_DisplayMode current;

		if (windowResolutions.size() > 0) {
			currentDisplay = windowResolutions[0];
			windowWidth = currentDisplay.w;
			windowHeight = currentDisplay.h;
		}
		//If there are displays get the resolution of the first one
		else if (displays > 0) {
			//This should be native display even if it goes fullscreen(vs SDL_GetDesktopDisplayMode())
			SDL_GetCurrentDisplayMode(0, &current);
			windowWidth = current.w;
			windowHeight = current.h;
			currentDisplay = current;
		}
		else {
			windowWidth = 1024;
			windowHeight = 576;
		}

		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowParameters);
	}
	//If failed to create window
	if (!window) {
		return false;
	}
	windowContext = SDL_GL_CreateContext(window);
	windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (windowRenderer == nullptr) {
		printf("Renderer could not be created! %s", SDL_GetError());
		Debug::SetSeverity(MessageType::TYPE_ERROR);
	
		std::string message = "Renderer could not be created!";
		message += SDL_GetError();
	
		Debug::Error(message, __FILE__, __LINE__);
	}

	//Creating a double buffer
	//Draw on back buffer display on front
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//V-sync
	SDL_GL_SetSwapInterval(vsync);
	//Remove mouse cursor and capture in window
	//SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetWindowGrab(window, SDL_TRUE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	eventFlags = SDL_WINDOWEVENT | SDL_WINDOWEVENT_SHOWN | SDL_WINDOWEVENT_EXPOSED | SDL_WINDOWEVENT_FOCUS_GAINED | SDL_WINDOWEVENT_ENTER | SDL_WINDOWEVENT_HIDDEN | SDL_WINDOWEVENT_FOCUS_LOST | SDL_WINDOWEVENT_LEAVE | SDL_WINDOWEVENT_SIZE_CHANGED | SDL_WINDOWEVENT_RESIZED | SDL_WINDOWEVENT_MAXIMIZED | SDL_WINDOWEVENT_RESTORED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_INPUT_GRABBED;
	MouseShowCursor = true; 
	SDL_Surface* surface = IMG_Load("Assets/Textures/Logos/pants.png");
	if (!surface) {

		printf("icon did not load.");

	}
	SDL_SetWindowIcon(window, surface);
	return true;
}

void Window::Update(SDL_Event& e)
{
	if((e.type & eventFlags) == e.type) {
		switch (e.window.event)
		{
		//case(SDL_WINDOW_INPUT_GRABBED):
		//case(SDL_WINDOW_INPUT_FOCUS):
		case(SDL_WINDOWEVENT_SHOWN)://Window has been shown, ex. alt + tab through applications
		case(SDL_WINDOWEVENT_ENTER)://Window has gained mouse focus, mouse enters(hovers over) window
		case(SDL_WINDOWEVENT_FOCUS_GAINED)://Window gained focused, this occurs before exposed
		case(SDL_WINDOWEVENT_EXPOSED)://Window is exposed and needs to be redrawn, is "selected"
			ShowCursor();
				
			
			break;
		case(SDL_WINDOWEVENT_HIDDEN):
		case(SDL_WINDOWEVENT_FOCUS_LOST):
		case(SDL_WINDOWEVENT_LEAVE)://Window has lost mouse focus
			SDL_ShowCursor(SDL_ENABLE);
			SDL_SetWindowGrab(window, SDL_FALSE);
			SDL_CaptureMouse(SDL_FALSE);
			SDL_SetRelativeMouseMode(SDL_FALSE);
			
			break;
		case(SDL_WINDOWEVENT_SIZE_CHANGED)://The window size has changed, either as a result of an API call or through the system or user changing the window size
		case(SDL_WINDOWEVENT_RESIZED):
			windowWidth = e.window.data1;
			windowHeight = e.window.data2;

			glViewport(0, 0, windowWidth, windowHeight);

			resizeCamera = true;
			//screenSurface = SDL_GetWindowSurface(window);
			break;
		case(SDL_WINDOWEVENT_MAXIMIZED):
			windowWidth = e.window.data1;
			windowHeight = e.window.data2;
			glViewport(0, 0, windowWidth, windowHeight);
			break;
			/*
			case(SDL_WINDOWEVENT_MINIMIZED):
				break;
			case(SDL_WINDOWEVENT_CLOSE)://Window manager requests window to be closed
				e.type = SDL_QUIT;
				SDL_PushEvent(&e);
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
inline void Window::ShowCursor() {
	
	if (MouseShowCursor) {

		SDL_ShowCursor(SDL_ENABLE);
		SDL_CaptureMouse(SDL_FALSE);
		SDL_SetWindowGrab(window, SDL_FALSE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		
	}
	else {

		SDL_ShowCursor(SDL_DISABLE);
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetWindowGrab(window, SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);

	}
}


void Window::Render()
{
	SDL_GL_SwapWindow(window);
}
bool Window::Shutdown()
{
	bool success = true;

	SDL_DestroyWindow(window);

	if (window) {
		success = false;
	}
	else {
		window = nullptr;
	}

	windowResolutions.clear();
	windowResolutions.shrink_to_fit();

	SDL_DestroyRenderer(windowRenderer);
	
	if (windowRenderer) {
		success = false;
	}
	else {
		windowRenderer = nullptr;
	}

	CloseSDL();

	return success;
}

void Window::SetWindowResolution(int index) {
	if (windowResolutions.size() < 0 && index < windowResolutions.size()) {
		currentDisplay = windowResolutions[index];
		SDL_SetWindowSize(window, currentDisplay.w, currentDisplay.h);
	}
}

void Window::SetWindowResolution(SDL_DisplayMode display) {

		currentDisplay = display;
		SDL_SetWindowSize(window, currentDisplay.w, currentDisplay.h);
}



bool Window::InitSDL() {
	bool success = true;
	//SDL_SetHint("SDL_HINT_GAMECONTROLLERCONFIG", "1");
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
	//Fullscreen
	if (!fullscreen) {
		windowParameters |= SDL_WINDOW_FULLSCREEN;
		fullscreen = true;
	}
	//Don't fullscreen
	else {
		windowParameters ^= SDL_WINDOW_FULLSCREEN;
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
		b = SDL_FALSE;
	}
	else {
		windowParameters ^= SDL_WINDOW_BORDERLESS;
		borderless = false;
		b = SDL_TRUE;
	}
	SDL_SetWindowBordered(window, b);
}

void Window::VSync()
{
	vsync = vsync ? false : true;
	SDL_GL_SetSwapInterval(vsync);
}

