#include "DeathBox.h"


DeathBox::DeathBox() : GameObject("DeathBox") {}
DeathBox::DeathBox(const std::string& name) : GameObject(name) {
	Tag = "DeathBox";

	rigidBody = new RigidBody(this, false);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, glm::vec3(-55.0f, -10.0f, -150.0f) , glm::vec3(55.0f, 10.0f, 150.0f));
	collider->isTrigger = true;

	transform.position = glm::vec3(0.0f, -25.0f, -50.0f);
	UpdateTransform();
}


DeathBox::~DeathBox() {}

void DeathBox::Update(float deltaTime) {
	UpdateTransform();
	collider->Update(deltaTime);
}
