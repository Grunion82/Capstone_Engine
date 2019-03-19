#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "../../Math/DList.h"

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

protected:
	Transform transform; 

	//GameObject* Parent;
	//DList<GameObject> Children;

	std::string Name;
	std::string Tag;
	__int16 Layer;

	bool isActive;

	void UpdateTransform();

public:

	//Set of Constructors for instantiation 
	GameObject(const std::string name = "GameObject", const std::string tag = "", const __int16 layer = 0, const vec3 p = vec3(0.0f, 0.0f, 0.0f), const vec3 s = vec3(1.0f), const vec3 r = vec3(0.0f, 1.0f, 0.0f), const float a = 0.0f);
	virtual ~GameObject();

	//Standard 4 Init, Update, Render, and Shutdown
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void Shutdown();

	//Returns member transform
	Transform GetTransform() const;

	//Sets the GameObject to be active or not
	void SetActive(const bool active);

	//Translate Methods
	void Translate(const vec3& value);
	void Translate(const float x, const float y, const float z);

	//Rotate Methods-- Axes must be nomralized
	void Rotate(const vec3& value);
	void Rotate(const float angle, vec3& axis);
	void Rotate(const float x, const float y, const float z);
	void Rotate(const float angle, const float x, const float y, const float z);

	//Scale Methods
	void Scale(const vec3& value);
	void Scale(const float x, const float y, const float z);
	void Scale(const float value);

	//Name Setter and Getter
	void SetName(const std::string& name);
	std::string GetName() const;

	//Tag Setter and Getter
	void SetTag(const std::string& tag);
	std::string GetTag() const;

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