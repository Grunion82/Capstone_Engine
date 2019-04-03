#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#define GRAVITY_CONSTANT -9.81f

#include <glm/glm.hpp>

//Structure for RigidBody component
struct RigidBody {
private:
	float bounciness;

public:
	bool isEnabled;
	bool isGravityEnabled;
	float mass;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::mat4 transform;

	class GameObject* referenceObject;

	//Default Constructor/Destructor
	RigidBody(class GameObject* referenceObj, bool grav = true);
	~RigidBody();

	//Updates transform with equations of motion (3D)
	void Update(const float deltaTime);

	//Sets is this RigidBody will use Gravity or not
	void SetUseGravity(bool willUseGravity);

	//Sets this RigidBody's mass
	void SetMass(float m);

	//Sets the bounciness factor of the RigidBody
	void SetBounciness(float b);

	//Retrieves the Bounciness attribute
	float GetBounciness() const;

	//Apply a force to this RigidBody
	void ApplyForce(glm::vec3 force);
};

#endif // ! RIGIDBODY_H