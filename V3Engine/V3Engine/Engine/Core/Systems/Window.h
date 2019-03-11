#ifndef WINDOW_H
#define WINDOW_H

#include "EventSystem.h"
#include <vector>

class Window : public EventSystem
{
	std::vector<SDL_DisplayMode> windowResolutions;
	~Window();

	//The actual window
	SDL_Window* window;
	//The current display mode
	SDL_DisplayMode currentDisplay;
	//The context of the window
	SDL_GLContext windowContext;

	const char* windowName;
	unsigned int windowWidth;
	unsigned int windowHeight;
	
	//Parameters of the window such as fullscreen or resizable
	unsigned int windowParameters;

	//Event flags for checking events regrading window
	int eventFlags;

	bool fullscreen = false;
	bool borderless = false;
	bool vsync = true;

	bool InitSDL();
	bool CloseSDL();

public:
	//Generic window will assume 800x600
	Window(const char* name);
	//Generic window based off resolution
	Window(unsigned int width,unsigned int height);
	//Window with specific parameters
	Window(const char* name, unsigned int width, unsigned int height);


	bool Init();
	void Update(SDL_Event& e) override;
	void Render();
	bool Shutdown() override;

	inline SDL_Surface* GetScreenSurface() { return SDL_GetWindowSurface(window); }
	inline SDL_Window* GetWindow() { return window; }
	inline int GetWidth() { return windowWidth; }
	inline int GetHeight() { return windowHeight; }
	inline std::vector<SDL_DisplayMode> GetWindowResolutions() { return windowResolutions; }
	inline SDL_DisplayMode GetCurrentResolution() { return currentDisplay; }
	inline SDL_DisplayMode GetResolution(unsigned int index) { if (&windowResolutions[index] != nullptr) { return windowResolutions[index]; } }
	inline SDL_GLContext GetContext() { return windowContext; }

	void Fullscreen();
	void Borderless();
	void VSync();
};

#endif