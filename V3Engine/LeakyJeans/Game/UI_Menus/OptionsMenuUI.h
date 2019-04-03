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
	float CalculateDisplayChange(float initial_, float final_);


	std::vector<SDL_DisplayMode> windowRes;
};
#endif // !OPTIONSMENU_H
