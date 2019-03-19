#include "V3Engine.h"

#include <SDL.h>

#include "Core/Managers/EventManager.h"
#include "Core/Systems/Window.h"
#include "Core/Systems/Input.h"
#include "Core/Systems/Debug.h"
#include "Core/Systems/Timer.h"
#include "Core/Systems/Camera.h"
#include "Core/Game/GameInterface.h"

#include "Rendering/Graphic.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/2D/Image.h"

std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

Camera* c;

V3Engine::V3Engine() : engineWindow(new Window("Leaky Jeans", 800, 600)) {
	Debug::DebugInit();
	engineWindow->Init();
	timer = new Timer();
	timer->Start();
	EventManager::GetInstace()->AddEventSystem<Window>(engineWindow);
	Graphic::GetInstance()->InitOpenGL();
	Input::GetInstance()->Init();
	Graphic::GetInstance()->Init(engineWindow);

	c = new Camera(engineWindow);
}

V3Engine::~V3Engine() {

}


V3Engine* V3Engine::GetInstance() {

	if (engineInstance.get() == nullptr)
		engineInstance.reset(new V3Engine);

	return engineInstance.get();

}

void V3Engine::speak() {	
	printf("Engine speaketh\n");	
}

void V3Engine::NewGame(GameInterface* game) {
	currentGame = game;
}

void V3Engine::Run() {
	while (!Input::GetInstance()->QuitRequested()) {
		timer->UpdateFrameTicks();

		EventManager::GetInstace()->Update();
		currentGame->Update(timer->GetDeltaTime());

		if (Input::GetInstance()->WasKeyPressed(SDLK_ESCAPE)) {
			break;
		}

		c->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		c->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);

		//c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer.GetDeltaTime());
		c->Update();

		Render();
		SDL_Delay(timer->GetSleepTime(90)); // 90 fps
	}

	Shutdown();
}

void V3Engine::Render()
{
	//Render
	//Graphic::GetInstance()->Update();

	//For geometry
	//Graphic::GetInstance()->GeometryPass();

	//For light
	//Graphic::GetInstance()->LightingPass();

	//Set light properties
	/*for (unsigned int i = 0; i < numLights; i++) {
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].position", lightPos[i]);
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].color", lightColor[i]);
		float constant = 1.0f;
		float linear = 0.7f;
		float quadratic = 1.8f;
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].linear", linear);
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);

		float maxBrightness = std::fmaxf(std::fmaxf(lightColor[i].r, lightColor[i].g), lightColor[i].b);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("light[" + std::to_string(i) + "].radius", radius);
	}
	Graphic::GetInstance()->gbuffer.lightShader->SetVec3("viewPos", c->GetPosition());

	//Render to quad
	Graphic::GetInstance()->Render();

	//Copy from gbuffer to default buffer
	Graphic::GetInstance()->gbuffer.BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), 0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);*/

	currentGame->Render();

	engineWindow->Render();
}

bool V3Engine::Shutdown() {
	EventManager::GetInstace()->Shutdown();
	Graphic::GetInstance()->Shutdown();

	delete currentGame;
	currentGame = nullptr;

	delete timer;
	timer = nullptr;

	delete engineWindow;
	engineWindow = nullptr;

	return true;
}
