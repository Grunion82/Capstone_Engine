#ifndef JEANS_H

#include <Engine/Core/Game/GameObject.h>

class Jeans : public GameObject {
private:
	bool hasJumped;
	float speed;

public:
	Jeans();
	Jeans(const std::string& name, glm::vec3 position);
	~Jeans();

	bool CanJump() { return hasJumped; }
	void SetHasJump(bool value) { hasJumped = true; }
	float JeanSpeed() { return speed; }
	void SetJeanSpeed(float value) { speed = value; }

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;

	void CollisionEnter(GameObject* collisionObj) override;
	void CollisionStay(GameObject* collisionObj) override;
	void CollisionExit(GameObject* collisionObj) override;
};

#endif // ! JEANS_H