#include "PhysicsManager.h"

#include "../Rendering/3D/GameObject.h"

//Global instantiation of static attributes
std::unique_ptr<PhysicsManager> PhysicsManager::physicsManagerInstance = nullptr;
std::vector<GameObject*> PhysicsManager::physicsObjects = std::vector<GameObject*>();

//Default Constructor/Destructor
PhysicsManager::PhysicsManager() {

}

PhysicsManager::~PhysicsManager() {
	for (GameObject* go : physicsObjects)
		go = nullptr;
}

//Updates Objects' RigidBodies
void PhysicsManager::UpdateRigidBodies(float deltaTime) {
	for (int i = 0; i < physicsObjects.size(); ++i) {
		if(physicsObjects[i]->GetRigidBody()->isEnabled)
			physicsObjects[i]->GetRigidBody()->Update(deltaTime);
	}
}

//Checks for Collisions between Objects
void PhysicsManager::CheckCollisions() {
	for (int i = 0; i < physicsObjects.size(); ++i) {
		for (int j = i + 1; j < physicsObjects.size(); ++j) {

			//RTTI definition for collision detection modes

			//Intersecting/Colliding colliders-- may need  "&& !foundCollision"
			if (physicsObjects[i]->GetBoundingBox().Intersect(&physicsObjects[j]->GetBoundingBox())) {
				printf("There was a collision\n");
				//printf("1 | 2 = %d\n", 1 | 2);
				CalculateCollisionResponse(i, j);
			}

		} // int j
	} // int i
}

//Calculates the response of the Collision between Objects
void PhysicsManager::CalculateCollisionResponse(int object1Index, int object2Index) {
	printf("%s and %s\n", physicsObjects[object1Index]->GetTag().c_str(), physicsObjects[object2Index]->GetTag().c_str());

	//Distance vector between positions
	glm::vec3 distance = physicsObjects[object2Index]->GetPosition() - physicsObjects[object1Index]->GetPosition();
	glm::vec3 normalizedDistance = glm::normalize(distance);

	//If the objects are still colliding but going away from each other, ignore response calculation
	glm::vec3 newVelocity = physicsObjects[object2Index]->GetRigidBody()->velocity - physicsObjects[object1Index]->GetRigidBody()->velocity;
	if (glm::dot(distance, newVelocity) < 0.0f) {
		//Conservation of Momentum and Co-efficient of Restitution to find collision response
		//Normalized Normal Vector
		glm::vec3 nN = glm::normalize(distance);

		//Initial Velocities of both Objects
		glm::vec3 Vi1 = physicsObjects[object1Index]->GetRigidBody()->velocity;
		glm::vec3 Vi2 = physicsObjects[object2Index]->GetRigidBody()->velocity;

		//The dot product for velocities
		float Vin1 = glm::dot(Vi1, nN);
		float Vin2 = glm::dot(Vi2, nN);

		//Masses for each object
		float m1 = physicsObjects[object1Index]->GetRigidBody()->mass;
		float m2 = physicsObjects[object2Index]->GetRigidBody()->mass;

		//Epsilon (Bounciness) of the 1st object
		float e = physicsObjects[object1Index]->GetRigidBody()->GetBounciness();

		//Coefficient of Restitution: Vf1 - Vf2 = -e(Vi1 - Vi2)
		//Conservation of Momentum: m1Vi1 - m2Vi2 = m1Vf1 - m2Vf2
		//Change in Velocity: dV = Vf1 - Vi1

		//Rearranged equations to find final Velocities along the normal
		float Vfn1 = ((m1 - e * m2) * Vin1 + (1 + e) * m2 * Vin2) / (m1 + m2);
		float Vfn2 = ((m2 - e * m1) * Vin2 + (1 + e) * m1 * Vin1) / (m1 + m2);

		//Converts the final velocities into 3d
		glm::vec3 Vf1 = Vi1 + (Vfn1 - Vin1) * nN;
		glm::vec3 Vf2 = Vi2 + (Vfn2 - Vin2) * nN;

		//Sets the new velocities accordingly
		if (physicsObjects[object1Index]->GetRigidBody()->isEnabled)
			physicsObjects[object1Index]->GetRigidBody()->velocity = Vf1;
		if (physicsObjects[object2Index]->GetRigidBody()->isEnabled)
			physicsObjects[object2Index]->GetRigidBody()->velocity = Vf2;
	}
}

//Retrieve the Instance
PhysicsManager* PhysicsManager::GetInstance() {
	if (physicsManagerInstance == nullptr) {
		physicsManagerInstance.reset(new PhysicsManager());
	}

	return physicsManagerInstance.get();
}

//Update the Object list
void PhysicsManager::Update(float deltaTime) {
	UpdateRigidBodies(deltaTime);
	CheckCollisions();
}

//Add Object to the list
void PhysicsManager::AddPhysicsObject(class GameObject* physObj) {
	physicsObjects.push_back(physObj);
}