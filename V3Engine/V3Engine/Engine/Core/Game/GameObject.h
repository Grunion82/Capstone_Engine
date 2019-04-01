#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Rendering/3D/Model.h"
#include "../../Math/Colliders.h"
#include "../../Math/RigidBody.h"

using namespace glm;

//Transform structure to hold a GameObjects position, scale, and rotation (in degrees)
struct Transform {
	mat4 TransformationMatrix;
	vec3 position;
	vec3 scale;
	vec3 rotation;
	float angle;
};

class GameObject {
private:
	//Default constructor-- Do not use
	GameObject();

	friend class PhysicsManager;
	friend class Collision;

protected:
	Model* model;
	Collider* collider;
	RigidBody* rigidBody;

	Transform transform; 

	//GameObject* Parent;
	//DList<GameObject> Children;

	std::string Name;
	std::string Tag;
	__int16 Layer;

	bool isActive;

	//Updates the GameObjects transform after moving it
	void UpdateTransform();
	
	virtual void CollisionEnter(GameObject* collisionObj);
	virtual void CollisionStay(GameObject* collisionObj);
	virtual void CollisionExit(GameObject* collisionObj);

public:

	//Set of Constructors for instantiation 
	GameObject(const std::string name, const std::string tag = "", const __int16 layer = 0, const vec3 p = vec3(0.0f, 0.0f, 0.0f), const vec3 s = vec3(1.0f), const vec3 r = vec3(0.0f, 1.0f, 0.0f), const float a = 0.0f);
	virtual ~GameObject();

	//Standard 4 Init, Update, Render, and Shutdown
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Render(const class Camera* camera);
	virtual void Shutdown();

	//Returns member transform
	Transform GetTransform() const;

	//Sets the GameObject to be active or not
	void SetActive(const bool active);

	//Translate Methods
	void Translate(const vec3& value);
	void Translate(const float x, const float y, const float z);

	void SetPos(const vec3& value) { transform.position = value; }
	void SetPos(const float x, const float y, const float z) { transform.position = glm::vec3(x, y, z); }

	//Rotate Methods-- Axes must be nomralized
	void Rotate(const vec3& value);
	void Rotate(const float angle, vec3& axis);
	void Rotate(const float x, const float y, const float z);
	void Rotate(const float angle, const float x, const float y, const float z);

	void SetRotate(const vec3& value) { transform.rotation = value; }
	void SetRotate(const float angle, vec3& axis) { transform.rotation = glm::eulerAngles(glm::quat(angle, axis)); }
	void SetRotate(const float x, const float y, const float z) { transform.rotation = glm::vec3(x, y, z); }
	void SetRotate(const float angle, const float x, const float y, const float z) { transform.rotation = glm::eulerAngles(glm::quat(angle, glm::vec3(x, y, z))); }

	//Scale Methods
	void Scale(const vec3& value);
	void Scale(const float x, const float y, const float z);
	void Scale(const float value);

	void SetScale(const vec3& value) { transform.scale = value; }
	void SetScale(const float x, const float y, const float z) { transform.scale = glm::vec3(x, y, z); }
	void SetScale(const float value) { transform.scale = glm::vec3(value, value, value); }

	//Name Setter and Getter
	void SetName(const std::string& name);
	std::string GetName() const;

	//Tag Setter and Getter
	void SetTag(const std::string& tag);
	std::string GetTag() const;

	//Retrieve Model Component
	inline Model* GetModel() { return model; }
	//Retrieve Collider Component
	inline Collider* GetCollider() { return collider; }
	//Retrieve Rigidbody Component
	inline RigidBody* GetRigidBody() { return rigidBody; }
	inline glm::mat4 GetTransformMatrix() { return transform.TransformationMatrix; }

	

	//Layer Setter and Getter
	/*void SetLayer(const __int16 layer);
	__int16 GetLayer() const;

	//Set and Retrieve Parent Object
	void SetParent(GameObject* parent);
	GameObject* GetParent() const;

	//Add to and Retrieve from Chilren List. Requires Dereference
	void AddChild(const GameObject child);
	DList<GameObject> GetChildren() const;*/
};

#endif // ! GAMEOBJECT_H