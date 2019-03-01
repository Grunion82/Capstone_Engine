#include "EventManager.h"
#include "../Systems/Input.h"
#include "../Systems/Window.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{

}

void EventManager::Update()
{
	Input::GetInstance()->Update();

	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		Input::GetInstance()->Update(e);
		for (unsigned int i = 0; i < eventSystems.size(); i++) {
			eventSystems[i]->Update(e);
		}
	}
}

void EventManager::Shutdown()
{
	Input::GetInstance()->Shutdown();
	for (unsigned int i = 0; i < eventSystems.size(); i++) {
		eventSystems[i]->Shutdown();
	}
}

void EventManager::AddEventSystem(EventSystem * es)
{
	eventSystems.push_back(es);
}
