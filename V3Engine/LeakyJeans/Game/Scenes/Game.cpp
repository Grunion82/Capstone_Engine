#include "Game.h"
#include "../Jeans.h"
#include "../Platform.h"
#include "../V3Engine/Engine/Core/Systems/Window.h"
#include "../V3Engine/Engine/Core/Systems/Input.h"
#include "../V3Engine/Engine/V3Engine.h"


Game::Game()
{

	c.reserve(2);
	c.push_back(new Camera(V3Engine::GetInstance()->GetEngineWindow()));
	c.push_back(new Camera(V3Engine::GetInstance()->GetEngineWindow()));

	c[0]->SetCameraViewport(0, V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);
	c[1]->Translate(glm::vec3(0.0f, 0.0f, 15.0f));
	c[1]->SetCameraViewport(0, 0, V3Engine::GetInstance()->GetEngineWindow()->GetWidth(), V3Engine::GetInstance()->GetEngineWindow()->GetHeight() / 2);

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
	AddGameObject(new Jeans("Jeans", glm::vec3(0.0f, 5.0f, -15.0f)));
	AddGameObject(new Platform("Platform1", glm::vec3(0.0f, -5.0f, -15.0f)));
	gameObjects["Jeans"]->SetCamera(c[0]);

}


Game::~Game()
{
	delete skybox;
	skybox = nullptr;
	for (unsigned int i = 0; i < c.size(); i++) {
		delete c[i];
		c[i] = nullptr;
	}

}

void Game::Update(float deltaTime)
{

	//c[0]->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), deltaTime);

	c[0]->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);

	c[0]->Controller(Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(0, SDL_CONTROLLER_AXIS_RIGHTY), deltaTime);
	c[1]->Controller(Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(1, SDL_CONTROLLER_AXIS_RIGHTY), deltaTime);

	for (unsigned int i = 0; i < c.size(); i++) {
		c[i]->Update();
	}

	skybox->Update(deltaTime);
	for (auto go : gameObjects) {
		go.second->Update(deltaTime);
	}

}

void Game::Render(const Camera * camera)
{

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

}
