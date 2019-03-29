#include "GameInterface.h"

#include "../../Core/Systems/Camera.h"

bool GameInterface::Init() {

	return false;
}

void GameInterface::Update(const float DeltaTime) {
	currentScene->Update(DeltaTime);
}

void GameInterface::Render(const Camera* camera) {
	currentScene->Render(camera);
}

void GameInterface::Shutdown() {
	if (scenes.size() > 0) {
		for (auto sc : scenes) {

			delete sc.second;
			sc.second = nullptr;
		}
		scenes.clear();
	}

	//delete currentScene;
	currentScene = nullptr;
}

void GameInterface::SetCurrentScene(std::string name) {
	if (scenes.find(name) != scenes.end()) {
		currentScene = scenes[name];
	}
}

void GameInterface::SetName(std::string name) {
	Name = name;
}

std::string GameInterface::GetName() const {
	return Name;
}

void GameInterface::AddScene(Scene* scene) {
	scenes[scene->GetName()] = scene;
	if (scenes.size() == 1) {
		SetCurrentScene(scene->GetName());
	}
}

void GameInterface::RemoveScene(std::string sceneName) {
	if (scenes.find(sceneName) != scenes.end()) {
		delete scenes[sceneName];
		scenes.erase(sceneName);
	}
}
