#include "GameObject.h"

void GameObject::UpdateTransform() {
	mat4 newTransform = glm::mat4();

	newTransform = glm::translate(newTransform, transform.position);
	mat4 translateInverse = glm::mat4();
	translateInverse = glm::translate(newTransform, transform.position);
	translateInverse = glm::inverse(translateInverse);
	newTransform = glm::rotate(newTransform, transform.angle, transform.rotation);
	newTransform = glm::scale(newTransform, transform.scale);

	transform.TransformationMatrix = newTransform;
	if (model)
		model->Update(0);
	if (collider)
		collider->Update(0);
	if (camera)
		camera->Translate(transform.position);
}

void GameObject::CollisionEnter(GameObject* collisionObj) {

}

void GameObject::CollisionStay(GameObject* collisionObj) {

}

void GameObject::CollisionExit(GameObject* collisionObj) {

}

GameObject::GameObject() {

}

GameObject::GameObject(const std::string name, const std::string tag, const __int16 layer, const vec3 p, const vec3 s, const vec3 r, const float a) {

	model = nullptr;
	collider = nullptr;
	rigidBody = nullptr;
	camera = nullptr;

	Name = name;
	Tag = tag;
	Layer = layer;
	transform.position = p;
	transform.scale = s;
	transform.rotation = r;
	transform.angle = a;
	UpdateTransform();
}

//Clean up pointers
GameObject::~GameObject() {
	
	Shutdown();
}

bool GameObject::Init() {

	return true;
}

void GameObject::Update(float deltaTime) {

	UpdateTransform();
}

void GameObject::Render(const Camera* camera) {

}

void GameObject::Shutdown() {

	if (model) {
		delete model;
		model = nullptr;
	}
	if (collider) {
		delete collider;
		collider = nullptr;
	}
	if (rigidBody) {
		delete rigidBody;
		rigidBody = nullptr;
	}

	//delete Parent;
	//Parent = nullptr;
}

Transform GameObject::GetTransform() const {

	return transform;
}

void GameObject::SetActive(const bool active) {

	isActive = active;
}

void GameObject::Translate(const vec3& value) {
	transform.position += value;
	if (camera) {
		camera->SetPosition(value);
	}
	UpdateTransform();
}

void GameObject::Translate(const float x, const float y, const float z) {

	transform.position += vec3(x, y, z);
	UpdateTransform();
}

void GameObject::Rotate(const vec3& value) {

	Rotate(value.x, value.y, value.z);
}

void GameObject::Rotate(const float angle, vec3& axis) {

	if (axis.length() != 1.0f) {
		axis = normalize(axis);
	}
	transform.angle = angle;
	Rotate(angle, axis.x, axis.y, axis.z);
}

void GameObject::Rotate(const float x, const float y, const float z) {
	
	transform.rotation += vec3(x, y, z);
	UpdateTransform();
}

void GameObject::Rotate(const float angle, const float x, const float y, const float z) {
	
	vec3 tempAxis = vec3(x, y, z);
	if (tempAxis.length() != 1.0f) {
		tempAxis = normalize(tempAxis);
	}

	transform.rotation += vec3(angle * tempAxis.x, angle * tempAxis.y, angle * tempAxis.z);
	UpdateTransform();
}

void GameObject::Scale(const vec3& value) {

	Scale(value.x, value.y, value.z);
}

void GameObject::Scale(const float x, const float y, const float z) {

	transform.scale.x *= x;
	transform.scale.y *= y;
	transform.scale.z *= z;
	UpdateTransform();
}

void GameObject::Scale(const float value) {

	transform.scale *= value;
	UpdateTransform();
}

void GameObject::SetName(const std::string& name) {

	Name = name;
}

std::string GameObject::GetName() const {

	return Name;
}

void GameObject::SetTag(const std::string& tag) {

	Tag = tag;
}

std::string GameObject::GetTag() const {

	return Tag;
}

/*void GameObject::SetLayer(const __int16 layer) {

	Layer = layer;
}

__int16 GameObject::GetLayer() const {

	return Layer;
}

void GameObject::SetParent(GameObject* parent) {

	Parent = parent;
}

GameObject* GameObject::GetParent() const {

	return Parent;
}

void GameObject::AddChild(const GameObject child) {

	Children.push_back(child);
}

DList<GameObject> GameObject::GetChildren() const {

	return Children;
}*/