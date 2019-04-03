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
	transform = referenceObject->GetTransformMatrix();
}