#ifndef JEANS_H

#include <Engine/Core/Game/GameObject.h>

class Jeans : public GameObject {
private:
	bool hasJumped;

public:
	Jeans();
	Jeans(const std::string& name, glm::vec3 position);
	~Jeans();

	virtual void Update(float deltaTime) override;
	virtual void Render(const class Camera* camera) override;
};

#endif // ! JEANS_H