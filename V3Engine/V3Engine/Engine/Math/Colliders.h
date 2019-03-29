#ifndef COLLIDERS_H
#define COLLIDERS_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define BOX_BOX_COLLISION 2
#define BOX_SPHERE_COLLISION 3
#define SPHERE_BOX_COLLISION 3
#define SPHERE_SPHERE_COLLISION 4

enum ColliderType {
	NONE = 0,
	BOX = 1,
	SPHERE = 2
};

//Abstract class for all children Colliders to inherit from
class Collider {
protected:
	ColliderType type;

	class GameObject* referenceObject;

public:
	Collider() {}
	virtual ~Collider() {}

	glm::mat4 transform;

	virtual void Update(float deltaTime) {}

	inline virtual ColliderType GetType() const {
		return type;
	}
};

//Structure for BoundingBox
struct BoundingBox : public Collider {
private:

public:
	float width;
	float height;
	float length;
	glm::vec3 minVert;
	glm::vec3 maxVert;

	inline BoundingBox() {
		type = ColliderType::BOX;
		minVert = glm::vec3(0.0f);
		maxVert = glm::vec3(0.0f);
		transform = glm::mat4(1.0f);
	}

	BoundingBox(class GameObject* refObj, glm::vec3 min, glm::vec3 max);
	~BoundingBox();

	inline void CalculateDimensions() {

		width = maxVert.x - minVert.x;
		height = maxVert.y - minVert.y;
		length = maxVert.z - minVert.z;
		//printf("Width: %.6f, Height: %.6f, Length: %.6f\n", width, height, length);
	}

	void Update(float deltaTime) override;

	inline glm::vec3 GetTransformPoint(glm::vec3 point, glm::mat4 transformation) {
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);

		return point + translation;
	}

	inline bool Intersect(BoundingBox* box) {
		glm::vec3 minCorner = GetTransformPoint(minVert, transform);
		glm::vec3 maxCorner = GetTransformPoint(maxVert, transform);
		glm::vec3 other_MinCorner = GetTransformPoint(box->minVert, box->transform);
		glm::vec3 other_MaxCorner = GetTransformPoint(box->maxVert, box->transform);

		return (minCorner.x < other_MaxCorner.x && maxCorner.x > other_MinCorner.x && minCorner.y < other_MaxCorner.y && maxCorner.y > other_MinCorner.y && minCorner.z < other_MaxCorner.z && maxCorner.z > other_MinCorner.z);
	}
};

//Structure for Sphere Collider
struct SphereCollider : public Collider {
private:

public:
	float radius;

	inline SphereCollider() {
		type = ColliderType::SPHERE;
		radius = 1.0f;
		transform = glm::mat4(1.0f);
	}
};

#endif // ! COLLIDERS_H