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
	if (collidingObjects.size() > 0) {
		for (auto c : collidingObjects) {
			c.second.referenceObject1 = nullptr;
			c.second.referenceObject2 = nullptr;
		}
		collidingObjects.clear();
	}
	if (wasCollidingObjects.size() > 0) {
		for (auto c : wasCollidingObjects) {
			c.second.referenceObject1 = nullptr;
			c.second.referenceObject2 = nullptr;
		}
		wasCollidingObjects.clear();
	}

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

			/*if (physicsObjects[i]->collider->GetType() == ColliderType::BOX && physicsObjects[j]->collider->GetType() == ColliderType::BOX) {
				if (BoxBoxCollision(dynamic_cast<BoundingBox*>(physicsObjects[i]->collider), dynamic_cast<BoundingBox*>(physicsObjects[j]->collider)))
					//printf("box box collision\n"); 
					NewCollision(i, j); CalculateCollisionResponse(i, j);
			}*/

			//Intersecting/Colliding colliders-- may need  "&& !foundCollision"
			if (dynamic_cast<BoundingBox*>(physicsObjects[i]->collider)->Intersect(dynamic_cast<BoundingBox*>(physicsObjects[j]->collider))) {
				NewCollision(i, j);
				CalculateCollisionResponse(i, j);
			}

		} // int j
	} // int i

	CheckOldCollisions();
}

//Detection between two Box Colliders
bool PhysicsManager::BoxBoxCollision(BoundingBox* object1, BoundingBox* object2) {
	//Vectors of vertices from each Box
	std::vector<glm::vec3> box1Verts = SetUpVertices(object1);
	std::vector<glm::vec3> box2Verts = SetUpVertices(object2);

	const int PROJECTION_AXIS_SIZE = 6;
	//Array of projection axes to compare with
	glm::vec3 projAxis[PROJECTION_AXIS_SIZE];

	//Axes of first object
	projAxis[0] = glm::normalize(box1Verts[1] - box1Verts[0]);
	projAxis[1] = glm::normalize(box1Verts[3] - box1Verts[0]);
	projAxis[2] = glm::normalize(box1Verts[5] - box1Verts[0]);
	//Axes of second object
	projAxis[3] = glm::normalize(box2Verts[1] - box2Verts[0]);
	projAxis[4] = glm::normalize(box2Verts[3] - box2Verts[0]);
	projAxis[5] = glm::normalize(box2Verts[5] - box2Verts[0]);

	float b1Min = 0.0f, b1Max = 0.0f;
	float b2Min = 0.0f, b2Max = 0.0f;
	float proj1 = 0.0f, proj2 = 0.0f;

	for (int i = 0; i < PROJECTION_AXIS_SIZE; ++i) {
		for (int j = 0; j < box1Verts.size(); ++j) {

			proj1 = glm::dot(box1Verts[j], projAxis[i]);
			proj2 = glm::dot(box2Verts[j], projAxis[i]);

			if (j == 0) {
				b1Min = b1Max = proj1;
				b2Min = b2Max = proj2;
			}
			else {
				if (proj1 < b1Min)
					b1Min = proj1;
				if (proj1 > b1Max)
					b1Max = proj1;

				if (proj2 < b2Min)
					b2Min = proj2;
				if (proj2 > b2Max)
					b2Max = proj2;
			}
		} // int j

		float highestMin = min(b1Min, b2Min);
		float lowestMax = max(b1Max, b2Max);

		float overlap = lowestMax - highestMin; 

		float halfDist1 = (b1Max - b1Min) * 0.5f;
		float midPoint1 = b1Min + halfDist1;

		float halfDist2 = (b2Max - b2Min) * 0.5f;
		float midPoint2 = b2Min + halfDist2;

		if (abs(midPoint1 - midPoint2) > (halfDist1 + halfDist2))
			return false;

	} // int i 

	return true;
}

//Detection between a Box and Sphere Collider
bool PhysicsManager::BoxSphereCollision(BoundingBox* object1, SphereCollider* object2) {

	return true;
}

//Detection between two Sphere Colliders
bool PhysicsManager::SphereSphereCollision(SphereCollider* object1, SphereCollider* object2) {

	return true;
}

//Determines the Vertices of a Bounding Box 
std::vector<glm::vec3> PhysicsManager::SetUpVertices(BoundingBox* box) {
	std::vector<glm::vec3> vertices = std::vector<glm::vec3>();

	vertices.push_back(box->maxVert);
	vertices.push_back(glm::vec3(box->minVert.x, box->maxVert.y, box->maxVert.z));
	vertices.push_back(glm::vec3(box->minVert.x, box->minVert.y, box->maxVert.z));
	vertices.push_back(glm::vec3(box->maxVert.x, box->minVert.y, box->maxVert.z));
	vertices.push_back(glm::vec3(box->maxVert.x, box->minVert.y, box->minVert.z));
	vertices.push_back(glm::vec3(box->maxVert.x, box->maxVert.y, box->minVert.z));
	vertices.push_back(glm::vec3(box->minVert.x, box->maxVert.y, box->minVert.z));
	vertices.push_back(box->minVert);

	return vertices;
}

//Checks for new Collisions
void PhysicsManager::NewCollision(int object1Index, int object2Index) {
	std::string signature = physicsObjects[object1Index]->Name + physicsObjects[object2Index]->Name;
	if (collidingObjects.find(signature) == collidingObjects.end()) {
		Collision c = Collision(physicsObjects[object1Index], physicsObjects[object2Index], signature);
		collidingObjects[signature] = c;
		c.CollisionEnter();
	}
	else {
		collidingObjects[signature].isActive = true;
	}
}

//Checks if any collisions are no longer present
void PhysicsManager::CheckOldCollisions() {
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
	//Distance vector between positions
	glm::vec3 position1 = physicsObjects[object1Index]->transform.position;
	glm::vec3 position2 = physicsObjects[object2Index]->transform.position;

	//Distance between the two objects' centers
	glm::vec3 distance = position2 - position1;

	//If the objects are still colliding but going away from each other, ignore response calculation
	glm::vec3 newVelocity = physicsObjects[object2Index]->rigidBody->velocity - physicsObjects[object1Index]->rigidBody->velocity;

	//Holds if the two objects are both active or only one is. True = both active, False = one inactive
	bool isSystemActive = physicsObjects[object1Index]->isActive & physicsObjects[object2Index]->isActive;

	if (glm::dot(distance, newVelocity) < 0.0f || !(isSystemActive)) {
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
		if (physicsObjects[object1Index]->rigidBody->isEnabled && isSystemActive)
			physicsObjects[object1Index]->rigidBody->velocity = Vf1;

		if (physicsObjects[object2Index]->rigidBody->isEnabled && isSystemActive)
			physicsObjects[object2Index]->rigidBody->velocity = Vf2;
		
		if(!physicsObjects[object1Index]->collider->isTrigger & !physicsObjects[object2Index]->collider->isTrigger)
			PositionalCorrection(object1Index, object2Index);
	}
}

//Corrects the position of overlapping objects
void PhysicsManager::PositionalCorrection(int object1Index, int object2Index) {
	glm::vec3 directionAxes[6];
	directionAxes[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	directionAxes[1] = glm::vec3(-1.0f, 0.0f, 0.0f);
	directionAxes[2] = glm::vec3(0.0f, 1.0f, 0.0f);
	directionAxes[3] = glm::vec3(0.0f, -1.0f, 0.0f);
	directionAxes[4] = glm::vec3(0.0f, 0.0f, 1.0f);
	directionAxes[5] = glm::vec3(0.0f, 0.0f, -1.0f);

	//Distance vector between positions
	glm::vec3 position1 = physicsObjects[object1Index]->transform.position;
	glm::vec3 position2 = physicsObjects[object2Index]->transform.position;

	//Distance between the two objects' centers
	glm::vec3 distance = position2 - position1;
	glm::vec3 nN = glm::normalize(distance);

	float inverseMassObj1 = (physicsObjects[object1Index]->rigidBody->mass < 0.0f) ? 0.0f : 1.0f / physicsObjects[object1Index]->rigidBody->mass;
	float inverseMassObj2 = (physicsObjects[object2Index]->rigidBody->mass < 0.0f) ? 0.0f : 1.0f / physicsObjects[object2Index]->rigidBody->mass;

	const float percent = 1.0f; //Reduce depth by Percent
	const float slop = 0.1f; //Arbitrary threshold penetration must pass
	float minObj1, maxObj1;
	float minObj2, maxObj2;

	BoundingBox* obj1Box = dynamic_cast<BoundingBox*>(physicsObjects[object1Index]->collider);
	BoundingBox* obj2Box = dynamic_cast<BoundingBox*>(physicsObjects[object2Index]->collider);

	for (int i = 0; i < 6; ++i) {
		switch (i) {

		//PosX
		case 0:
			maxObj1 = position1.x + obj1Box->width / 2.0f;
			minObj2 = position2.x - obj2Box->width / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && maxObj1 - 0.5f < minObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj1 - minObj2; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate((-inverseMassObj1) * correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.x > 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.x = 0.0f;
			}

		//Neg X
		case 1:
			minObj1 = position1.x - obj1Box->width / 2.0f;
			maxObj2 = position2.x + obj2Box->width / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && minObj1 + 0.5f > maxObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj2 - minObj1; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate(-(inverseMassObj1) * correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.x < 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.x = 0.0f;
			}

		//Pos Y
		case 2:
			maxObj1 = position1.y + obj1Box->height / 2.0f;
			minObj2 = position2.y - obj2Box->height / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && maxObj1 - 0.5f < minObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj1 - minObj2; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate((inverseMassObj1)* correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.y > 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.y = 0.0f;
			}

		//Neg Y
		case 3:
			minObj1 = position1.y - obj1Box->height / 2.0f;
			maxObj2 = position2.y + obj2Box->height / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && minObj1 + 0.5f > maxObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj2 - minObj1; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate(-(inverseMassObj1)* correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.y < 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.y = 0.0f;
			}

		//Pos Z
		case 4:
			maxObj1 = position1.z + obj1Box->width / 2.0f;
			minObj2 = position2.z - obj2Box->width / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && maxObj1 - 0.5f < minObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj1 - minObj2; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate((inverseMassObj1)* correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.z > 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.z = 0.0f;
			}

		//Neg Z
		case 5:
			minObj1 = position1.z - obj1Box->width / 2.0f;
			maxObj2 = position2.z + obj2Box->width / 2.0f;

			if (glm::dot(physicsObjects[object1Index]->rigidBody->velocity, directionAxes[i]) > 0.0f && minObj1 + 0.5f > maxObj2) {
				//Positional Correction- Sinking Objects
				const float penetration = maxObj2 - minObj1; //Penetration depth

				glm::vec3 correction = std::max((penetration - slop), 0.0f) / (inverseMassObj1 + inverseMassObj2) * percent * nN; //Correction vector

				physicsObjects[object1Index]->Translate(-(inverseMassObj1)* correction);
				if (physicsObjects[object1Index]->rigidBody->velocity.z < 0.0f)
					physicsObjects[object1Index]->rigidBody->velocity.z = 0.0f;
			}

		default:
			break;
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