#include "MainMenu.h"

#include "Jeans.h"
#include "Platform.h"

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
	AddGameObject(new Jeans("Jeans", glm::vec3(0.0f, 3.0f, -5.0f)));
	AddGameObject(new Platform("Platform1", glm::vec3(0.0f, 0.0f, -5.0f)));
}


MainMenu::~MainMenu() {
	delete skybox;
	skybox = nullptr;
}

void MainMenu::Update(float deltaTime) {
	skybox->Update(deltaTime);
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
