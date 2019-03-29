#ifndef V3ENGINE_H
#define V3ENGING_H

#include <memory>
#include <stdio.h>
#include <SDL.h>

class V3Engine {
private:
	V3Engine();
	~V3Engine();
	class Window* engineWindow;
	class GameInterface* currentGame;

	class Timer* timer;

	static std::unique_ptr<V3Engine> engineInstance;
public:
	
	V3Engine(const V3Engine&) = delete;
	V3Engine(V3Engine&&) = delete;
	V3Engine& operator=(const V3Engine&) = delete;
	V3Engine& operator=(V3Engine&&) = delete;

	friend std::default_delete<V3Engine>;

	static V3Engine* GetInstance();
	void speak();

	void NewGame(class GameInterface* game);
	void Run();

	bool Init();
	void Update();
	void Render();
	bool Shutdown(); 
};

#endif // ! V3ENGINE_H