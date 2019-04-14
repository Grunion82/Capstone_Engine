#include "OptionsMenu.h"

#include "../../../V3Engine/Engine/Core/Game/GameInterface.h"


OptionsMenu::OptionsMenu(GameInterface* ref)
{
	Name = "Options Menu";
	//create init inmediateUI*
	iu = new OptionsMenuUI(ref);

	iu->Initialize();

	//set initial UI state
	iu->UIstate = GameState::Options_Menu;
	uiState = iu->UIstate;
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


OptionsMenu::~OptionsMenu()
{

	delete iu;
	iu = nullptr;
	delete w;
	w = nullptr;

}

void OptionsMenu::Update(float deltaTime)
{

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

void OptionsMenu::Render(const Camera * camera)
{
}

void OptionsMenu::Render()
{

	if (iu != nullptr) {

		iu->Render();

	}

}
