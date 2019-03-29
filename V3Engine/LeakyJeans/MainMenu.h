#ifndef MAINMENU_H
#define MAINMENU_H

#include <Engine/Core/Game/Scene.h>

class MainMenu : public Scene {
private:
	Skybox* skybox;

public:
	MainMenu();
	~MainMenu();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;
};

#endif // ! MAINMENU_H