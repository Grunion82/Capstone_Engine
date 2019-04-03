#ifndef JEANS_H

#include <Engine/Core/Game/GameObject.h>

class Jeans : public GameObject {
private:
	bool hasJumped;
	const float MAX_SPEED = 15.0f;
	float speed;
	float drainRatio;
	const float MAX_WATER_LEVEL = 100.0f;
	float waterLevel;

public:
	Jeans();
	Jeans(const std::string& name, glm::vec3 position);
	~Jeans();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;

	void CollisionEnter(GameObject* collisionObj) override;
	void CollisionStay(GameObject* collisionObj) override;
	void CollisionExit(GameObject* collisionObj) override;

	int GetWaterLevel() const;
};

#endif // ! JEANS_H