#include "Colliders.h"

#include "../Core/Game/GameObject.h"

BoundingBox::BoundingBox(GameObject* refObj, glm::vec3 min, glm::vec3 max) {
	type = ColliderType::BOX;
	isTrigger = false;
	referenceObject = refObj;
	minVert = min;
	maxVert = max;
	transform = referenceObject->GetTransform().TransformationMatrix;
	CalculateDimensions();
}

BoundingBox::~BoundingBox() {
	referenceObject = nullptr;
}

void BoundingBox::Update(float deltaTime) {
	transform = referenceObject->GetTransform().TransformationMatrix;

	//Transforms the minVert based off of referenceObject's transform
	//glm::vec4 tempVert = glm::vec4(minVert, 1.0f);
	//tempVert = tempVert * transform;
	//minVert = glm::vec3(tempVert);
	////Transforms the maxVert based off of referenceObject's transform
	//tempVert = glm::vec4(maxVert, 1.0f);
	//tempVert = tempVert * transform;
	//maxVert = glm::vec3(tempVert);
}