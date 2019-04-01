#include "PhysicsManager.h"

#include <algorithm>

//Global instantiation of static attributes
std::unique_ptr<PhysicsManager> PhysicsManager::physicsManagerInstance = nullptr;
std::vector<GameObject*> PhysicsManager::physicsObjects = std::vector<GameObject*>();
std::map<std::string, Collision> PhysicsManager::collidingObjects = std::map<std::string, Collision>();
std::map<std::string, Collision> PhysicsManager::wasCollidingObjects = std::map<std::string, Collision>();

//Default Constructor/Destructor
PhysicsManager::PhysicsManager() {

}

PhysicsManager::~PhysicsManager() {
	if(collidingObjects.size() > 0)
		collidingObjects.clear();
	if(wasCollidingObjects.size() > 0)
		wasCollidingObjects.clear();

	for (GameObject* go : physicsObjects)
		go = nullptr;
}

//Updates Objects' RigidBodies
void PhysicsManager::UpdateRigidBodies(float deltaTime) {
	for (int i = 0; i < physicsObjects.size(); ++i) {
		if (physicsObjects[i]->rigidBody->isEnabled)
			physicsObjects[i]->rigidBody->Update(deltaTime);
	}
}

//Checks for Collisions between Objects
void PhysicsManager::CheckCollisions() {
	for (int i = 0; i < physicsObjects.size(); ++i) {
		for (int j = i + 1; j < physicsObjects.size(); ++j) {

			//RTTI definition for collision detection modes

			//Intersecting/Colliding colliders-- may need  "&& !foundCollision"
<<<<<<< HEAD
			if (dynamic_cast<BoundingBox*>(physicsObjects[i]->GetCollider())->Intersect(dynamic_cast<BoundingBox*>(physicsObjects[j]->GetCollider()))) {
				//printf("There was a collision\n");
				//printf("get angle: %p\n", physicsObjects[i]->Update);
				//printf("1 | 2 = %d\n", 1 | 2);
=======
			if (dynamic_cast<BoundingBox*>(physicsObjects[i]->collider)->Intersect(dynamic_cast<BoundingBox*>(physicsObjects[j]->collider))) {
				std::string signature = physicsObjects[i]->Name + physicsObjects[j]->Name;
				if (collidingObjects.find(signature) == collidingObjects.end()) {
					Collision c = Collision(physicsObjects[i], physicsObjects[j], signature);
					collidingObjects[signature] = c;
					c.CollisionEnter();
				}
				else {
					collidingObjects[signature].isActive = true;
				}

>>>>>>> origin/grunion
				CalculateCollisionResponse(i, j);
			}

		} // int j
	} // int i

	wasCollidingObjects = collidingObjects;
	for (auto c : collidingObjects) {
		if (c.second.isActive) {
			c.second.CollisionStay();
		}
		else {
			c.second.CollisionExit();
			wasCollidingObjects.erase(c.second.signature);
		}
		if (wasCollidingObjects.find(c.second.signature) != wasCollidingObjects.end()) {
			wasCollidingObjects[c.second.signature].isActive = false;
		}
	}
	collidingObjects = wasCollidingObjects;
}

//Calculates the response of the Collision between Objects
void PhysicsManager::CalculateCollisionResponse(int object1Index, int object2Index) {
<<<<<<< HEAD
	//printf("%s and %s\n", physicsObjects[object1Index]->GetName().c_str(), physicsObjects[object2Index]->GetName().c_str());

=======
	
>>>>>>> origin/grunion
	//Distance vector between positions
	glm::vec3 position1 = physicsObjects[object1Index]->transform.position;
	glm::vec3 position2 = physicsObjects[object2Index]->transform.position;

	//Distance between the two objects' centers
	glm::vec3 distance = position2 - position1;

	//If the objects are still colliding but going away from each other, ignore response calculation
	glm::vec3 newVelocity = physicsObjects[object2Index]->rigidBody->velocity - physicsObjects[object1Index]->rigidBody->velocity;
	if (glm::dot(distance, newVelocity) < 0.0f) {
		//Conservation of Momentum and Co-efficient of Restitution to find collision response
		//Normalized Distance Vector
		glm::vec3 nN = glm::normalize(distance);

		//Initial Velocities of both Objects
		glm::vec3 Vi1 = physicsObjects[object1Index]->rigidBody->velocity;
		glm::vec3 Vi2 = physicsObjects[object2Index]->rigidBody->velocity;

		//The dot product for velocities
		float Vin1 = glm::dot(Vi1, nN);
		float Vin2 = glm::dot(Vi2, nN);

		//Masses for each object
		float m1 = physicsObjects[object1Index]->rigidBody->mass;
		float m2 = physicsObjects[object2Index]->rigidBody->mass;

		//Epsilon (Bounciness) of the 1st object
		float e = physicsObjects[object1Index]->rigidBody->GetBounciness();

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
		if (physicsObjects[object1Index]->rigidBody->isEnabled)
			physicsObjects[object1Index]->rigidBody->velocity = Vf1;

		if (physicsObjects[object2Index]->rigidBody->isEnabled)
			physicsObjects[object2Index]->rigidBody->velocity = Vf2;

		//Down axis
		glm::vec3 downAxis = glm::vec3(0.0f, -1.0f, 0.0f);

		//Bottom of First object and Top of Second
		float minObj1 = position1.y - dynamic_cast<BoundingBox*>(physicsObjects[object1Index]->collider)->height / 2.0f;
		float maxObj2 = position2.y + dynamic_cast<BoundingBox*>(physicsObjects[object2Index]->collider)->height / 2.0f;

		if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, downAxis) > 0.0f && minObj1 < maxObj2) {
			//Positional Correction- Sinking Objects
			float inverseMassObj1 = (physicsObjects[object1Index]->rigidBody->mass < 0.0f) ? 0.0f : 1.0f / physicsObjects[object1Index]->rigidBody->mass;
			float inverseMassObj2 = (physicsObjects[object2Index]->rigidBody->mass < 0.0f) ? 0.0f : 1.0f / physicsObjects[object2Index]->rigidBody->mass;

			const float percent = 1.0f; //Reduce depth by Percent
			const float slop = 0.1f; //Arbitrary threshold penetration must pass

			const float penetration = maxObj2 - minObj1; //Penetration depth

			glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

			physicsObjects[object1Index]->Translate((-inverseMassObj1) * correction);
			if(physicsObjects[object1Index]->rigidBody->velocity.y < 0.0f)
				physicsObjects[object1Index]->rigidBody->isEnabled = false;

		}
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