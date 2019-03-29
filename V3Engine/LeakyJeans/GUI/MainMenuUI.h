
#ifndef MAINMENUUI_H
#define MAINMENUUI_H

#include <Engine/Core/InmediateUI/InmediateUI.h>

class MainMenuUI : public InmediateUI
{
public:

	MainMenuUI();
	~MainMenuUI();

	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_); 
	void MainMenuOptions();

	void WindowStyle(ImGuiStyle* ref);

};
#endif MAINMENUUI_H

