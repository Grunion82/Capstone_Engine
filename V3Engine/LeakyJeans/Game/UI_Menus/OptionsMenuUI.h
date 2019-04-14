#ifndef OPTIONSMENUUI_H
#define OPTIONSMENUUI_H

#include <Engine/Core/InmediateUI/InmediateUI.h>



class OptionsMenuUI : public InmediateUI
{
private:
	class GameInterface* gameRef;

public:
	OptionsMenuUI(){};
	OptionsMenuUI(class GameInterface* ref);
	~OptionsMenuUI();

	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_);
	void WindowStyle(ImGuiStyle* ref);
	ImVec2 CalculateDisplayChange(float initial_x, float final_x, float initial_y, float final_y);


	std::vector<SDL_DisplayMode> windowRes;
};
#endif // !OPTIONSMENU_H
