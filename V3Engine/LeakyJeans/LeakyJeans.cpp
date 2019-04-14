#include "LeakyJeans.h"

#include "Game.h"
#include "Game/Scenes/MainMenu.h"
#include "Game/Scenes/OptionsMenu.h"

LeakyJeans::LeakyJeans() {
	Name = "Leaky Jeans";
	type = GameState::Main_Menu;

	AddScene(new MainMenu(this));
	AddScene(new OptionsMenu(this));
	AddScene(new Game());
}


LeakyJeans::~LeakyJeans() {

}

void LeakyJeans::Update(float deltaTime) {
	currentScene->Update(deltaTime);

	if (Input::GetInstance()->GetJoystickButton(0, SDL_CONTROLLER_BUTTON_START) || Input::GetInstance()->GetJoystickButton(1, SDL_CONTROLLER_BUTTON_START)) {
		type = GameState::Main_Menu;
	}

	switch (type) {
	case GameState::Main_Menu: 
		SetCurrentScene("Main Menu");
		break;
	case GameState::Options_Menu:
		SetCurrentScene("Options Menu");
		break;
	case GameState::In_Game: 
		SetCurrentScene("Game");
		break;
	default:
		break;
	}
}
