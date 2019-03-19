#include "RigidBody.h"

#include "../Rendering/3D/GameObject.h"

//Default Constructor/Destructor
RigidBody::RigidBody(GameObject* referenceObj, bool grav) {
	isEnabled = true;
	isGravityEnabled = grav;
	mass = 1.0f;
	bounciness = 0.0f;
	velocity = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
	transform = glm::mat4(1.0f);
	referenceObject = referenceObj;

}

RigidBody::~RigidBody() {
	referenceObject = nullptr;
}

//Updates transform with equations of motion (3D)
void RigidBody::Update(const float deltaTime) {
	if (isGravityEnabled) {
		ApplyForce(glm::vec3(0.0f, GRAVITY_CONSTANT, 0.0f));
	}

	velocity += acceleration * deltaTime;

	glm::vec3 displacement = (velocity * deltaTime) + 0.5f * (acceleration * (deltaTime * deltaTime));

	//Update reference object and transform's values
	referenceObject->SetPosition(referenceObject->GetPosition() + displacement);
	transform = referenceObject->GetBoundingBox().transform;
}

//Sets is this RigidBody will use Gravity or not
void RigidBody::SetGravity(bool willUseGravity) {
	isGravityEnabled = willUseGravity;
}

//Sets this RigidBody's mass
void RigidBody::SetMass(float m) {
	mass = m;
}

//Sets the bounciness factor of the RigidBody
void RigidBody::SetBounciness(float b) {
	//Limits factor to inbetween 1 and 0
	b = glm::clamp(b, 0.0f, 1.0f);

	bounciness = b;
}

float RigidBody::GetBounciness() const {
	return bounciness;
}

//Apply a force to this RigidBody
void RigidBody::ApplyForce(glm::vec3 force) {
	acceleration += force / mass;
}
