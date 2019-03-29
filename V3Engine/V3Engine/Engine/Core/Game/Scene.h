#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>

#include "Skybox.h"

class Scene {
protected:
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
	virtual void Shutdown();

	void SetName(std::string name);
	std::string GetName() const;

	//Adds, removes, and finds game objects within our map
	void AddGameObject(class GameObject* object);
	void RemoveGameObject(const std::string name);

	class GameObject* Find(const std::string name);
	
};

#endif // ! SCENE_H