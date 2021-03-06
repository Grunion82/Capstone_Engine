#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <map>

#include "Scene.h"
#include "../InmediateUI/InmediateUI.h"

class GameInterface {
protected:
	GameState type;
	//Scenes container of the game, Currently active scene, Game Name
	std::map<std::string, Scene*> scenes;
	Scene* currentScene;
	std::string Name;

public:
	GameInterface() {}
	virtual ~GameInterface() { Shutdown(); }

	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void Render(const class Camera* camera);
	virtual void Render();
	virtual void Shutdown();

	void SetCurrentScene(std::string name);

	void SetName(std::string name);
	std::string GetName() const;

	void SetState(GameState s) { type = s; }
	GameState GetState() const { return type; }

	void AddScene(Scene* scene);
	void RemoveScene(std::string sceneName);
};

#endif // ! GAMEINTERFACE_H