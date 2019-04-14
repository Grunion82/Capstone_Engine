#ifndef IMGAMEUI_H
#define INGAMEUI_H

#include <Engine/Core/InmediateUI/InmediateUI.h>
#include "../../Game.h"

class InGameUI : public InmediateUI
{
private: 
	class GameInterface* gameRef;
	Game* g;
	InmediateUI* iu;
	std::vector<Jeans*> players;
public:

	InGameUI();
	InGameUI(Game* game);
	~InGameUI();


	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_);
	void MainMenuOptions();

	void WindowStyle(ImGuiStyle* ref);
	float CalculateDisplayChange(float initial_, float final_);

	void SetGame(Game* game) { g = game; }
	Game* GetGame() { g; }

	void SetInmediateUI(InmediateUI* iUI) { iu = iUI; }
	InmediateUI* GetInmediateUI() { return iu; }
};

#endif // !IMGAMEUI_H

