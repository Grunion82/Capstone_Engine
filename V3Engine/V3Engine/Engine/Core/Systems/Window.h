#ifndef WINDOW_H
#define WINDOW_H

struct SDL_Window;
struct SDL_Surface;

class Window
{
public:
	Window();
	Window(char* name, unsigned int width, unsigned int height);
	~Window();

	SDL_Window* window;
	SDL_Surface* screenSurface;

	const char* windowName;
	const unsigned int windowWidth;
	const unsigned int windowHeight;
	bool fullscreen = false;
	bool borderless = false;

	unsigned int windowParameters;

	bool InitSDL();
	bool CloseSDL();

public:
	bool Init();
	void Update();
	void Render();
	bool Shutdown();

	inline SDL_Surface* GetScreenSurface() { return screenSurface; }
	inline SDL_Window* GetWindow() { return window; }
	inline float GetWidth();
	inline float GetHeight();

	void Fullscreen();
};

#endif