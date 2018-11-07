#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class Window
{
	~Window();

	SDL_Window* window;

	const char* windowName;
	unsigned int windowWidth;
	unsigned int windowHeight;
	unsigned int defaultWidth;
	unsigned int defaultHeight;

	unsigned int windowParameters;

	bool fullscreen = false;
	bool borderless = false;
	bool vsync = true;

	bool InitSDL();
	bool CloseSDL();

public:
	Window();
	Window(const char* name, unsigned int width, unsigned int height);

	bool Init();
	void Update();
	void Render();
	bool Shutdown();

	inline SDL_Surface* GetScreenSurface() { return SDL_GetWindowSurface(window); }
	inline SDL_Window* GetWindow() { return window; }
	inline int GetWidth() { return windowWidth; }
	inline int GetHeight() { return windowHeight; }

	void Fullscreen();
	void Borderless();
	void VSync();
};

#endif