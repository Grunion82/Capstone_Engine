#ifndef SCENE_H
#define SCENE_H

#include <vector>

class Scene {
protected:
	//Vector of game objects within the scene
	std::vector<class GameObject*> gameObjects;

public:
	Scene();
	virtual ~Scene();

	//Standard 4 Init, Update, Render, and Shutdown
	virtual bool Init();
	virtual void Update();
	virtual void Render();
	virtual void Shutdown();

	//Adds, removes, and finds game objects within our vector
	void AddGameObject(GameObject* object);
	void AddGameObject(unsigned int index, GameObject* object);
	void RemoveGameObject(const unsigned int index);

	class GameObject* Find(const unsigned int index);
	
};

#endif // ! SCENE_H