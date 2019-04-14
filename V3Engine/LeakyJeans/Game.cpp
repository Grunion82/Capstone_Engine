#include "Game.h"


#include "Platform.h"
#include "Spikes.h"
#include "HydrationStation.h"
#include "Flag.h"
#include "ScoreManager.h"
#include "DeathBox.h"

#include <Engine/Core/Systems/Window.h>
#include <Engine/Core/Systems/Input.h>
#include <Engine/V3Engine.h>
#include "Game/UI_Menus/InGameUI.h"
#include "Game/UI_Menus/OptionsMenuUI.h"

Game::Game() {
	float jeansStartY = 10.0f;

	c.reserve(2);
	c.push_back(new Camera(V3Engine::GetInstance()->GetEngineWindow()));
	c.push_back(new Camera(V3Engine::GetInstance()->GetEngineWindow()));
	c[0]->SetCameraViewport(0, V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);
	c[0]->SetPosition(glm::vec3(-5.0f, jeansStartY, 0.0f));
	c[1]->SetCameraViewport(0, 0, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);
	c[1]->SetPosition(glm::vec3(5.0f, jeansStartY, 0.0f));

	Name = "Game";

	std::vector<const char*> tempFaces{
	"Assets/SkyboxMap/skyrenderRight.bmp",
	"Assets/SkyboxMap/skyrenderLeft.bmp",
	"Assets/SkyboxMap/skyrenderUp.bmp",
	"Assets/SkyboxMap/skyrenderDown.bmp",
	"Assets/SkyboxMap/skyrenderFront.bmp",
	"Assets/SkyboxMap/skyrenderBack.bmp" };

	Shader* skyboxShader = new Shader("Assets/SkyboxMap/skyboxVert.glsl", "Assets/SkyboxMap/skyboxFrag.glsl");

	skybox = new Skybox(tempFaces, "Assets/SkyboxMap/cube.obj", skyboxShader);
#pragma region Jeans Start-Up   
	AddGameObject(new Jeans("Jeans", glm::vec3(-5.0f, jeansStartY, -15.0f)));
	gameObjects["Jeans"]->GetRigidBody()->isEnabled = false;
	gameObjects["Jeans"]->AddChild(new GameObject("Camera", "", 0, glm::vec3(-5.0f, jeansStartY, -15.0f)));
	gameObjects["Jeans"]->GetChild(0)->AttachCamera(c[0]);


	AddGameObject(new Jeans("Jeans2", glm::vec3(5.0f, jeansStartY, -15.0f)));
	gameObjects["Jeans2"]->GetRigidBody()->isEnabled = false;
	gameObjects["Jeans2"]->AddChild(new GameObject("Camera", "", 0, glm::vec3(5.0f, jeansStartY, -15.0f)));
	gameObjects["Jeans2"]->GetChild(0)->AttachCamera(c[1]);
#pragma endregion The initialization code for both Jeans objects

#pragma region DeathBox Start-Up
	AddGameObject(new DeathBox("DeathBox1"));
#pragma endregion The initialization code for the DeathBox

#pragma region Stage 1
	AddGameObject(new Platform("Platform1", glm::vec3(-5.0f, 0.0f, -15.0f)));
	AddGameObject(new Platform("Platform5", glm::vec3(5.0f, 0.0f, -15.0f)));
#pragma endregion

#pragma region Stage 2
	AddGameObject(new Platform("Platform2", glm::vec3(0.0f, 5.0f, -30.0f)));
	AddGameObject(new Spikes("Spikes1", glm::vec3(0.0f, 5.0f, -30.0f)));
#pragma endregion

#pragma region Stage 3
	AddGameObject(new Platform("Platform3", glm::vec3(-13.0f, 10.0f, -45.0f)));
	AddGameObject(new HydrationStation("HydrationStation1", glm::vec3(-13.0f, 10.25f, -45.0f)));
#pragma endregion

#pragma region Stage 4
	AddGameObject(new Platform("Platform4", glm::vec3(0.0f, 5.0f, -60.0f)));
	AddGameObject(new Spikes("Spikes2", glm::vec3(-2.0f, 5.0f, -60.0f)));
	AddGameObject(new Spikes("Spikes3", glm::vec3(2.0f, 5.0f, -60.0f)));
#pragma endregion

#pragma region Stage 5
	AddGameObject(new Platform("Platform6", glm::vec3(-7.0f, 5.0f, -75.0f)));
	AddGameObject(new Platform("Platform7", glm::vec3(7.0f, 8.0f, -75.0f)));
	AddGameObject(new Spikes("Spikes4", glm::vec3(7.0f, 8.0f, -75.0f)));
#pragma endregion

#pragma region Stage 6
	AddGameObject(new Platform("Platform8", glm::vec3(-12.0f, 9.0f, -90.0f)));
	AddGameObject(new Spikes("Spikes5", glm::vec3(-12.0f, 9.0f, -90.0f)));
	AddGameObject(new Platform("Platform9", glm::vec3(7.0f, 0.0f, -90.0f)));
	AddGameObject(new HydrationStation("HydrationStation2", glm::vec3(7.0f, 0.0f, -90.0f)));
#pragma endregion

#pragma region Stage 7
	AddGameObject(new Platform("Platform10", glm::vec3(-5.0f, 3.0f, -105.0f)));
	AddGameObject(new Spikes("Spikes6", glm::vec3(-5.0f, 3.0f, -105.0f)));
	AddGameObject(new Platform("Platform11", glm::vec3(5.0f, 3.0f, -105.0f)));
	AddGameObject(new Spikes("Spikes7", glm::vec3(5.0f, 3.0f, -105.0f)));
#pragma endregion

#pragma region Stage 8/Final
	AddGameObject(new Platform("Platform12", glm::vec3(0.0f, 0.0f, -130.0f)));
	AddGameObject(new Flag("FinishFlag", glm::vec3(0.0f, 0.1f, -130.0f)));
#pragma endregion

	//ScoreManager::ResetTime();
	//iu = new OptionsMenuUI();
	iu = new InGameUI(this);
	iu->SetWindow(c[0]->GetWindowToRender());
	iu->Initialize();
}


Game::~Game() {
	delete skybox;
	skybox = nullptr;

	for (unsigned int i = 0; i < c.size(); i++) {
		delete c[i];
		c[i] = nullptr;
	}
}

void Game::Update(float deltaTime) {

	if (Input::GetInstance()->GetJoystickButton(0, SDL_CONTROLLER_BUTTON_A) && !static_cast<Jeans*>(gameObjects["Jeans"])->CanJump()) {
		gameObjects["Jeans"]->GetRigidBody()->ApplyForce(glm::vec3(0.0f, 125.0f, 0.0f));
		static_cast<Jeans*>(gameObjects["Jeans"])->SetHasJump(true);

	}
	if (Input::GetInstance()->GetJoystickButton(1, SDL_CONTROLLER_BUTTON_A) && !static_cast<Jeans*>(gameObjects["Jeans2"])->CanJump()) {
		gameObjects["Jeans2"]->GetRigidBody()->ApplyForce(glm::vec3(0.0f, 125.0f, 0.0f));

		static_cast<Jeans*>(gameObjects["Jeans2"])->SetHasJump(true);
	}

	static_cast<Jeans*>(gameObjects["Jeans"])->MoveJeans(glm::vec3(Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_LEFTX), 0.0f, Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_LEFTY)), deltaTime);
	static_cast<Jeans*>(gameObjects["Jeans2"])->MoveJeans(glm::vec3(Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_LEFTX), 0.0f, Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_LEFTY)), deltaTime);


	for (unsigned int i = 0; i < c.size(); i++) {
		if (V3Engine::GetInstance()->GetEngineWindow()->resizeCamera) {
			c[0]->SetCameraViewport(0, V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);
			c[1]->SetCameraViewport(0, 0, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);

			V3Engine::GetInstance()->GetEngineWindow()->resizeCamera = false;
		}
		c[i]->Update();
	}

	skybox->Update(deltaTime);
	ScoreManager::GetInstance()->Update(deltaTime);
	for (auto go : gameObjects) {
		go.second->Update(deltaTime);
	}

	iu->Update(deltaTime);

}

void Game::Render(const Camera* camera) {
	skybox->Render(camera);
	for (auto go : gameObjects) {
		go.second->Render(camera);
	}
	
}

void Game::Render()
{
	for (unsigned int i = 0; i < c.size(); i++) {
		c[i]->Render();

		skybox->Render(c[i]);
		for (auto go : gameObjects) {
			go.second->Render(c[i]);
		}
	}

	iu->Render();
}
