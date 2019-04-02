#include "V3Engine.h"

#include <SDL.h>
#include "Core/Managers/EventManager.h"
#include "Core/Systems/Window.h"
#include "Core/Systems/Input.h"
#include "Core/Systems/Debug.h"
#include "Core/Systems/Timer.h"
#include "Core/Systems/Camera.h"
#include "Core/Game/GameInterface.h"

#include "Math/PhysicsManager.h"

#include "Rendering/Graphic.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/2D/Image.h"

std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

//std::vector<Camera*> c;

Timer timer;

V3Engine::V3Engine() : engineWindow(new Window("Leaky Jeans",1024,768)) {
	Debug::DebugInit();
	engineWindow->Init();
	timer = new Timer();
	timer->Start();
	EventManager::GetInstace()->AddEventSystem<Window>(engineWindow);
	Graphic::GetInstance()->InitOpenGL();
	Input::GetInstance()->Init();
	Graphic::GetInstance()->Init(engineWindow);
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
		PhysicsManager::GetInstance()->Update(timer->GetDeltaTime());

		if (Input::GetInstance()->WasKeyPressed(SDLK_ESCAPE)) {
			break;
		}

		//c[0]->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		//c[0]->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);
		
		//c[0]->Controller(Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());
		//c[1]->Controller(Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());

		//for (unsigned int i = 0; i < c.size(); i++) {
		//	c[i]->Update();
		//}

		//c->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		//c->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);

		//c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());
		//c->Update();

		Render();
		//SDL_Delay(timer->GetSleepTime(90)); // 90 fps
	}

	Shutdown();
}

void V3Engine::Render() {
	Graphic::GetInstance()->Update();
	//Graphic::GetInstance()->GeometryPass();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//for (unsigned int i = 0; i < c.size(); i++) {
		//c[i]->Render();
		currentGame->Render();
	//}

	//Graphic::GetInstance()->Render();
	//Graphic::GetInstance()->gbuffer.BindForReading();
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), 0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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