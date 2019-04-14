#include "GameObject.h"

void GameObject::UpdateTransform() {
	mat4 newTransform = glm::mat4();

	newTransform = glm::translate(newTransform, transform.position);
	newTransform = glm::rotate(newTransform, transform.angle, transform.rotation);
	newTransform = glm::scale(newTransform, transform.scale);

	transform.TransformationMatrix = newTransform;

	newTransform = glm::mat4();
	newTransform = glm::translate(newTransform, transform.localPosition);
	newTransform = glm::rotate(newTransform, transform.localAngle, transform.localRotation);
	newTransform = glm::scale(newTransform, transform.localScale);

	transform.LocalTransformationMatrix = newTransform;

	if (model)
		model->Update(0);
	if (collider)
		collider->Update(0);
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

	mat4 localMat = mat4();

	localMat = mat4();
	localMat = glm::translate(localMat, transform.position);
	localMat = glm::inverse(localMat);
	transform.localPosition = vec3(localMat * vec4(transform.position,1.0f));

	localMat = mat4();
	localMat = glm::rotate(localMat,transform.angle,transform.rotation);
	localMat = glm::inverse(localMat);
	transform.localRotation = vec3(localMat * vec4(transform.rotation,1.0f));

	localMat = mat4();
	localMat = glm::scale(localMat, transform.scale);
	localMat = glm::inverse(localMat);
	transform.localScale = vec3(localMat * vec4(transform.scale, 1.0f));
	transform.localAngle = 0.0f;

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

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->UpdateTransform();
			}
		}
	}
}

void GameObject::Render(const Camera* camera) {
	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Render(camera);
			}
		}
	}
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

	//if (camera) {
	//	delete camera;
	//	camera = nullptr;
	//}

	parentObject = nullptr;

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Shutdown();
				delete childObjects[i];
				childObjects[i] = nullptr;
			}
		}
	}
}

Transform GameObject::GetTransform() const {

	return transform;
}

void GameObject::SetActive(const bool active) {

	isActive = active;
}

void GameObject::Translate(const vec3& value) {
	transform.position += value;
	LocalTranslate(value);

	if (camera) {
		camera->Translate(value);
	}

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Translate(value);
			}
		}
	}
}

void GameObject::Translate(const float x, const float y, const float z) {
	transform.position += vec3(x, y, z);
	LocalTranslate(x, y, z);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Translate(x, y, z);
			}
		}
	}
}

void GameObject::LocalTranslate(const vec3 & value)
{
	transform.localPosition += value;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Translate(value);
	//		}
	//	}
	//}
}

void GameObject::LocalTranslate(const float x, const float y, const float z)
{
	transform.localPosition += vec3(x, y, z);

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->LocalTranslate(x, y, z);
	//		}
	//	}
	//}
}

void GameObject::Rotate(const vec3& value) {

	transform.rotation += value;
	LocalRotate(value);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Rotate(value);
			}
		}
	}
}

void GameObject::Rotate(const float angle, vec3& axis) {

	if (axis.length() != 1.0f) {
		axis = normalize(axis);
	}
	transform.angle = angle;
	transform.rotation += angle * axis;
	LocalRotate(angle, axis);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Rotate(angle,axis);
			}
		}
	}
}

void GameObject::Rotate(const float x, const float y, const float z) {
	transform.rotation += vec3(x, y, z);
	LocalRotate(x, y, z);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Rotate(x, y, z);
			}
		}
	}
}

void GameObject::Rotate(const float angle, const float x, const float y, const float z) {
	vec3 tempAxis = vec3(x, y, z);
	if (tempAxis.length() != 1.0f) {
		tempAxis = normalize(tempAxis);
	}

	transform.rotation += angle * tempAxis;
	LocalRotate(angle, x, y, z);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Rotate(angle,x, y, z);
			}
		}
	}
}

void GameObject::LocalRotate(const vec3& value) {
	transform.localRotation += value;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Rotate(value);
	//		}
	//	}
	//}
}

void GameObject::LocalRotate(const float angle, vec3& axis) {
	if (axis.length() != 1.0f) {
		axis = normalize(axis);
	}
	transform.localAngle = angle;
	transform.localRotation += angle * axis;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->LocalRotate(angle, axis);
	//		}
	//	}
	//}
}

void GameObject::LocalRotate(const float x, const float y, const float z) {
	transform.localRotation += vec3(x, y, z);

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->LocalRotate(x, y, z);
	//		}
	//	}
	//}
}

void GameObject::LocalRotate(const float angle, const float x, const float y, const float z) {
	vec3 tempAxis = vec3(x, y, z);
	if (tempAxis.length() != 1.0f) {
		tempAxis = normalize(tempAxis);
	}

	transform.localRotation += angle * tempAxis;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Rotate(angle, x, y, z);
	//		}
	//	}
	//}
}

void GameObject::Scale(const vec3& value) {
	transform.scale *= value;
	LocalScale(value);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Scale(value);
			}
		}
	}
}

void GameObject::Scale(const float x, const float y, const float z) {
	transform.scale.x *= x;
	transform.scale.y *= y;
	transform.scale.z *= z;
	LocalScale(x, y, z);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Scale(x, y, z);
			}
		}
	}
}

void GameObject::Scale(const float value) {
	transform.scale *= value;
	LocalScale(value);

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Scale(value);
			}
		}
	}
}

void GameObject::LocalScale(const vec3 & value)
{
	transform.scale *= value;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Scale(value);
	//		}
	//	}
	//}
}

void GameObject::LocalScale(const float x, const float y, const float z)
{
	transform.scale.x *= x;
	transform.scale.y *= y;
	transform.scale.z *= z;

	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Scale(x, y, z);
	//		}
	//	}
	//}
}

void GameObject::LocalScale(const float value)
{
	transform.scale *= value;
	//if (childObjects.size() > 0) {
	//	for (unsigned int i = 0; i < childObjects.size(); i++) {
	//		if (childObjects[i]) {
	//			childObjects[i]->Scale(value);
	//		}
	//	}
	//}
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