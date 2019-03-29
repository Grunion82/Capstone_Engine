#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <Engine/Core/InmediateUI/InmediateUI.h>



class OptionsMenu : public InmediateUI
{
public:
	OptionsMenu();
	~OptionsMenu();

	void imGuiScene(const char* frame_name_);
	void Update(const float deltaTime_);
};
#endif // !OPTIONSMENU_H
