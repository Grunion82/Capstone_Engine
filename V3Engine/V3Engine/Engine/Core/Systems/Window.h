#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <vector>


class Window
{
	std::vector<SDL_DisplayMode> windowResolutions;
	~Window();

	SDL_Window* window;
	SDL_DisplayMode currentDisplay;

	const char* windowName;
	unsigned int windowWidth;
	unsigned int windowHeight;

	unsigned int windowParameters;

	bool fullscreen = false;
	bool borderless = false;
	bool vsync = true;

	bool InitSDL();
	bool CloseSDL();

public:
	Window(const char* name);
	Window(unsigned int width,unsigned int height);
	Window(const char* name, unsigned int width, unsigned int height);

	bool Init();
	void Update(SDL_Event& e);
	void Render();
	bool Shutdown();

	inline SDL_Surface* GetScreenSurface() { return SDL_GetWindowSurface(window); }
	inline SDL_Window* GetWindow() { return window; }
	inline int GetWidth() { return windowWidth; }
	inline int GetHeight() { return windowHeight; }
	inline std::vector<SDL_DisplayMode> GetWindowResolutions() { return windowResolutions; }
	inline SDL_DisplayMode GetResolution(unsigned int index) { if (&windowResolutions[index] != nullptr) { return windowResolutions[index]; } }

	void Fullscreen();
	void Borderless();
	void VSync();
};

#endif