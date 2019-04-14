#ifndef MAINMENUUI_H
#define MAINMENUUI_H

#include <Engine/Core/InmediateUI/InmediateUI.h>

class Window;

class MainMenuUI : public InmediateUI
{
private:
	class GameInterface* gameRef;

public:
	MainMenuUI() {}
	MainMenuUI(class GameInterface* ref);
	~MainMenuUI();

	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_); 
	void MainMenuOptions();

	void WindowStyle(ImGuiStyle* ref);
	ImVec2 CalculateDisplayChange(float initial_x, float final_x,float initial_y, float final_y);
private:
	Window* win;
};
#endif MAINMENUUI_H

