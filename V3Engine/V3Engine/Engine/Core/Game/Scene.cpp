#include "Scene.h"

#include "../../Core/Systems/Camera.h"
#include "GameObject.h"

Scene::Scene() {

}

Scene::~Scene() {

	Shutdown();
}

bool Scene::Init() {

	return true;
}

void Scene::Update(float deltaTime) {

	for (auto go : gameObjects) {
		
		go.second->Update(deltaTime);
	}
}

void Scene::Render(const Camera* camera) {

	for (auto go : gameObjects) {

		go.second->Render(camera);
	}
}

void Scene::Shutdown() {

	if (gameObjects.size() > 0) {
		for (auto go : gameObjects) {

			delete go.second;
			go.second = nullptr;
		}
		gameObjects.clear();
	}
}

void Scene::SetName(std::string name) {
	Name = name;
}

std::string Scene::GetName() const {
	return Name;
}

void Scene::AddGameObject(GameObject* object) {

	gameObjects[object->GetName()] = object;
}

void Scene::RemoveGameObject(const std::string name) {

	if (gameObjects.find(name) != gameObjects.end()) {
		delete gameObjects[name];
		gameObjects.erase(name);
	}
}

GameObject* Scene::Find(const std::string name) {

	if (gameObjects.find(name) != gameObjects.end())
		return gameObjects[name];

	return new GameObject("BlankGameObject");
}