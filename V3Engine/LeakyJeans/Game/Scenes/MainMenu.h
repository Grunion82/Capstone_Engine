#ifndef MAINMENU_H
#define MAINMENU_H

#include <Engine/Core/Game/Scene.h>
#include "../UI_Menus/MainMenuUI.h"
#include "../UI_Menus/OptionsMenuUI.h"

class MainMenu : public Scene {
private:
	//Skybox* skybox;
	//std::vector<Camera*>c;
	InmediateUI * iu;
	GameState uiState;
	Window* w;
public:
	MainMenu();
	~MainMenu();

	void Update(float deltaTime) override;
	void Render(const Camera* camera) override;
	void Render() override;
};

#endif // ! MAINMENU_H