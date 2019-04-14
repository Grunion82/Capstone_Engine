#ifndef FLAG_H
#define FLAG_H

#include <Engine/Core/Game/GameObject.h>

class Flag : public GameObject {
private:

public:
	Flag();
	Flag(const std::string& name, glm::vec3 position);
	~Flag();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;
};

#endif // !FLAG_H