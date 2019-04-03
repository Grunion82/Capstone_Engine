#ifndef LEAKYJEANS_H
#define LEAKYJEANS_H

#include <Engine/Core/Game/GameInterface.h>

class LeakyJeans : public GameInterface {
private:
	class Scene* mainMenu;
	class Scene* optionsMenu;

public:
	LeakyJeans();
	~LeakyJeans();
	
	void Update(float deltaTime) override;
};

#endif // ! LEAKYJEANS_H