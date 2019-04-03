#ifndef MAINMENUUI_H
#define MAINMENUUI_H

#include <Engine/Core/InmediateUI/InmediateUI.h>

class Window;

class MainMenuUI : public InmediateUI
{
public:

	MainMenuUI();
	~MainMenuUI();

	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_); 
	void MainMenuOptions();

	void WindowStyle(ImGuiStyle* ref);
	float CalculateDisplayChange(float initial_, float final_);
private:
	Window* win;
};
#endif MAINMENUUI_H

