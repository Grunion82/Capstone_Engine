#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <memory>
#include <vector>
#include <map>

#include "../Core/Game/GameObject.h"

//Collision struct to hold the objects in a collision system
class Collision {
private:
	GameObject* referenceObject1;
	GameObject* referenceObject2;
	//Unique ID
	std::string signature;
	bool isActive;

	//Constructor to allow variable assignment. Private because only PhysicsManager should be able to create it.
	Collision(GameObject* ref1, GameObject* ref2, const std::string& sign) {
		referenceObject1 = ref1;
		referenceObject2 = ref2;
		signature = sign;
		isActive = true;
	}
	//Calls Enter for both references
	void CollisionEnter() {
		referenceObject1->CollisionEnter(referenceObject2);
		referenceObject2->CollisionEnter(referenceObject1);
	}
	//Calls Stay for both references
	void CollisionStay() {
		referenceObject1->CollisionStay(referenceObject2);
		referenceObject2->CollisionStay(referenceObject1);
	}
	//Calls Exit for both references
	void CollisionExit() {
		referenceObject1->CollisionExit(referenceObject2);
		referenceObject2->CollisionExit(referenceObject1);
	}
	
	friend class PhysicsManager;
public:
	//Default constructor-- Does nothing
	Collision() {}
	//Destructor-- Sets reference pointers to null
	~Collision() { referenceObject1 = nullptr; referenceObject2 = nullptr; }

};

class PhysicsManager {
private:
	//Map of colliding objects
	static std::map<std::string, Collision> collidingObjects;
	//Map of Past colliding objects
	static std::map<std::string, Collision> wasCollidingObjects;

	//List of Objects 
	static std::vector<GameObject*> physicsObjects;

	//Turns true once a single collision was found
	bool foundCollision;

	//Default Constructor/Destructor
	PhysicsManager();
	~PhysicsManager();

	//Updates Objects' RigidBodies
	void UpdateRigidBodies(float deltaTime);

	//Checks for Collisions between Objects
	void CheckCollisions();

	//Calculates the response of the Collision between Objects
	void CalculateCollisionResponse(int object1Index, int object2Index);

	//Instance of the PhysicsManager
	static std::unique_ptr<PhysicsManager> physicsManagerInstance;
	friend std::default_delete<PhysicsManager>;

public:
	//Standard 4 delete lines
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager(PhysicsManager&&) = delete;
	PhysicsManager& operator=(const PhysicsManager&) = delete;
	PhysicsManager& operator=(PhysicsManager&&) = delete;

	//Retrieve the Instance
	static PhysicsManager* GetInstance();

	//Update the Object list
	void Update(float deltaTime);

	//Add Object to the list
	void AddPhysicsObject(GameObject* physObj);

};

#endif // ! PHYSICSMANAGER_H