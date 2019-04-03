#include "MainMenu.h"

#include "../Jeans.h"
#include "../Platform.h"
#include "../V3Engine/Engine/Core/Systems/Window.h"
#include "../V3Engine/Engine/Core/Systems/Input.h"
#include "../V3Engine/Engine/V3Engine.h"


MainMenu::MainMenu() {


	
	//create init inmediateUI*
	iu = new MainMenuUI();

	iu->Initialize();

	//set initial UI state
	iu->UIstate = GameState::Main_Menu;
	uiState = iu->UIstate;

	//images
	TextureHandler::GetInstance()->CreateTexture("LeakyLogo", "Game/Assets/Textures/Logos/LeakyLogo.png");
	TextureHandler::GetInstance()->CreateTexture("EngineLogo", "Assets/Textures/Logos/EngineLogo.png");
	//background
	TextureHandler::GetInstance()->CreateTexture("Background_4", "Assets/Textures/Backgrounds/14.jpg");
	TextureHandler::GetInstance()->CreateTexture("Background_1", "Assets/Textures/Backgrounds/6.jpg");

	//sprites
	TextureHandler::GetInstance()->CreateTexture("DancingAnim", "Game/Assets/2dSprites/Dancing.png");
	TextureHandler::GetInstance()->CreateTexture("KickAnim", "Assets/2dSprites/Kick.png");
	TextureHandler::GetInstance()->CreateTexture("WiggleAnim", "Assets/2dSprites/Wiggle.png");
	TextureHandler::GetInstance()->CreateTexture("JumpAnim", "Assets/2dSprites/Jump.png");
	TextureHandler::GetInstance()->CreateTexture("WaterAnim", "Assets/2dSprites/water.png");

}


MainMenu::~MainMenu() {

	delete iu;
	iu = nullptr;
	delete w;
	w = nullptr;

}

void MainMenu::Update(float deltaTime) {



	if (iu != nullptr) {


		if (uiState != iu->UIstate) {
			uiState = iu->UIstate;
		}

		switch (uiState)
		{
		case GameState::Main_Menu:

			//check for the class the pointer is looking so we don't delete it every frame
			if (!dynamic_cast<MainMenuUI*>(iu)) {
				delete iu;
				iu = nullptr;
				iu = new MainMenuUI();
				iu->Initialize();
			}


			
			break;

		case GameState::Options_Menu:


			if (!dynamic_cast<OptionsMenuUI*>(iu)) {

				delete iu;
				iu = nullptr;
				iu = new OptionsMenuUI();
				iu->Initialize();
			}


			
			uiState = GameState::Options_Menu;
			iu->UIstate = GameState::Options_Menu;

			break;
		default:
			break;
		}

		iu->Update(deltaTime);

	}


}

void MainMenu::Render(const Camera* camera) {




}

void MainMenu::Render()
{


	if (iu != nullptr) {

		iu->Render();

	}
}
