#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <memory>
#include <vector>

class PhysicsManager {
private:
	//List of Objects 
	static std::vector<class GameObject*> physicsObjects;

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
	void AddPhysicsObject(class GameObject* physObj);

};

#endif // ! PHYSICSMANAGER_H