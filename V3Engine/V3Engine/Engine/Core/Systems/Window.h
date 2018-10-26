#ifndef WINDOW_H
#define WINDOW_H

struct SDL_Window;
struct SDL_Surface;

class Window
{
	~Window();

	SDL_Window* window;
	SDL_Surface* screenSurface;

	const char* windowName;
	const unsigned int windowWidth;
	const unsigned int windowHeight;

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

	inline SDL_Surface* GetScreenSurface() { return screenSurface; }
	inline SDL_Window* GetWindow() { return window; }
	inline int GetWidth();
	inline int GetHeight();

	void Fullscreen();
	void Borderless();
};

#endif