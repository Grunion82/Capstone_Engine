#include "Scene.h"


Scene::Scene() {

}

Scene::~Scene() {

	Shutdown();
}

bool Scene::Init() {

	return true;
}

void Scene::Update() {

	for (GameObject* go : gameObjects) {
		
		//g->Update();
	}
}

void Scene::Render() {

	for (GameObject* go : gameObjects) {

		//g->Render();
	}
}

void Scene::Shutdown() {

	for (GameObject* go : gameObjects) {

		//delete g;
		//g = nullptr;
	}
	gameObjects.clear();
}

void Scene::AddGameObject(class GameObject* object) {

	gameObjects.push_back(object);
}

void Scene::AddGameObject(unsigned int index, GameObject * object)
{
	gameObjects.insert(gameObjects.begin() + index, object);
}

void Scene::RemoveGameObject(const unsigned int index) {

	gameObjects.erase(gameObjects.begin() + index);
	gameObjects.shrink_to_fit();
}

GameObject* Scene::Find(const unsigned int index) {

	return gameObjects.at(index);
}