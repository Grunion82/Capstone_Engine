#ifndef PLATFORM_H
#define PLATFORM_H

#include <Engine/Core/Game/GameObject.h>

class Platform : public GameObject {
private:

public:
	Platform();
	Platform(const std::string& name, glm::vec3 position);
	~Platform();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;
};

#endif // !PLATFORM_H