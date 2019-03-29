#include "RigidBody.h"

#include "../Core/Game/GameObject.h"
#include "PhysicsManager.h"

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

	PhysicsManager::GetInstance()->AddPhysicsObject(referenceObject);

}

RigidBody::~RigidBody() {
	referenceObject = nullptr;
}

//Updates transform with equations of motion (3D)
void RigidBody::Update(const float deltaTime) {
	if (isGravityEnabled && acceleration.y > GRAVITY_CONSTANT) {
		float difference = GRAVITY_CONSTANT - glm::clamp(acceleration.y, GRAVITY_CONSTANT, 0.0f);
		ApplyForce(glm::vec3(0.0f, difference, 0.0f));
	}

	velocity += acceleration * deltaTime;

	glm::vec3 displacement = (velocity * deltaTime) + 0.5f * (acceleration * (deltaTime * deltaTime));

	//Update reference object and transform's values
	referenceObject->Translate(displacement);
	transform = referenceObject->GetTransform().TransformationMatrix;
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
	isEnabled = true;
}
