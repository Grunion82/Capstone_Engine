#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <Engine/Core/Game/Scene.h>
#include  "../UI_Menus/OptionsMenuUI.h"
#include "../UI_Menus/MainMenuUI.h"


class OptionsMenu : public Scene
{
private:
	InmediateUI * iu;
	GameState uiState;
	Window* w;
public:
	OptionsMenu(){}
	OptionsMenu(class GameInterface* ref);
	~OptionsMenu();

	void Update(float deltaTime) override;
	void Render(const Camera* camera) override;
	void Render() override;

};

#endif // !OPTIONSMENU_H