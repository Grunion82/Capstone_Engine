#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "Skybox.h"
#include "../InmediateUI/InmediateUI.h"

class Scene {
protected:
	class Camera* sceneCamera;
	//Map of game objects within the scene
	std::map<std::string, class GameObject*> gameObjects;
	std::string Name;

public:
	Scene();
	virtual ~Scene();

	//Standard 4 Init, Update, Render, and Shutdown
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Render(const class Camera* camera);
	virtual void Render();
	virtual void Shutdown();

	void SetName(std::string name);
	std::string GetName() const;

	//Adds, removes, and finds game objects within our map
	void AddGameObject(class GameObject* object);
	void RemoveGameObject(const std::string name);
	std::map<std::string, class GameObject*> GetGameObjects() { return gameObjects; }
	void SetGameObjects(std::map<std::string, class GameObject*> newObjects) { gameObjects = newObjects; }

	Camera* GetSceneCamera() { return sceneCamera; }
	void SetSceneCamera(Camera* c) { sceneCamera = c; }

	class GameObject* Find(const std::string name);
	
};

#endif // ! SCENE_H