#ifndef DEATHBOX_H
#define DEATHBOX_H

#include <Engine/Core/Game/GameObject.h>

class DeathBox : public GameObject {
private:

public:
	DeathBox();
	DeathBox(const std::string& name);
	~DeathBox();

	void Update(float deltaTime) override;
};

#endif // !DEATHBOX_H