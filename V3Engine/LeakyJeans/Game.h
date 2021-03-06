#ifndef GAME_H
#define GAME_H

#include <Engine/Core/Game/Scene.h>

class Game : public Scene {
private:
	Skybox* skybox;
	std::vector<Camera*>c;
public:
	Game();
	~Game();

	void Update(float deltaTime) override;
	void Render(const Camera* camera) override;
	void Render() override;
};

#endif // ! GAME_H