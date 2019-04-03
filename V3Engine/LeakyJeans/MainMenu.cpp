#include "MainMenu.h"

#include "Jeans.h"
#include "Platform.h"
#include "Spikes.h"
#include "HydrationStation.h"
#include "Flag.h"
#include "ScoreManager.h"

MainMenu::MainMenu() {
	Name = "Main Menu";

	std::vector<const char*> tempFaces{
	"Assets/SkyboxMap/skyrenderRight.bmp",
	"Assets/SkyboxMap/skyrenderLeft.bmp",
	"Assets/SkyboxMap/skyrenderUp.bmp",
	"Assets/SkyboxMap/skyrenderDown.bmp",
	"Assets/SkyboxMap/skyrenderFront.bmp",
	"Assets/SkyboxMap/skyrenderBack.bmp" };

	Shader* skyboxShader = new Shader("Assets/SkyboxMap/skyboxVert.glsl", "Assets/SkyboxMap/skyboxFrag.glsl");

	skybox = new Skybox(tempFaces, "Assets/SkyboxMap/cube.obj", skyboxShader);
	AddGameObject(new Jeans("Jeans", glm::vec3(0.0f, 50.0f, -15.0f)));
	AddGameObject(new Platform("Platform1", glm::vec3(0.0f, 0.0f, -15.0f)));
	AddGameObject(new Platform("Platform2", glm::vec3(0.0f, 5.0f, -30.0f)));
	AddGameObject(new Platform("Platform3", glm::vec3(-13.0f, 10.0f, -45.0f)));
	AddGameObject(new Platform("Platform4", glm::vec3(0.0f, 5.0f, -60.0f)));
	AddGameObject(new Spikes("Spikes1", glm::vec3(0.0f, 5.0f, -30.0f)));
	AddGameObject(new HydrationStation("HydrationStation1", glm::vec3(-13.0f, 10.25f, -45.0f)));
	AddGameObject(new Flag("FinishFlag", glm::vec3(0.0f, 5.0f, -60.0f)));
}


MainMenu::~MainMenu() {
	delete skybox;
	skybox = nullptr;
}

void MainMenu::Update(float deltaTime) {
	skybox->Update(deltaTime);
	ScoreManager::GetInstance()->Update(deltaTime);
	for (auto go : gameObjects) {
		go.second->Update(deltaTime);
	}

}

void MainMenu::Render(const Camera* camera) {
	skybox->Render(camera);
	for (auto go : gameObjects) {
		go.second->Render(camera);
	}

}
