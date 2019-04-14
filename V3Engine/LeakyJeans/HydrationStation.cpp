#include "HydrationStation.h"



HydrationStation::HydrationStation() : GameObject("HydrationStation") {

}

HydrationStation::HydrationStation(const std::string& name, glm::vec3 position) : GameObject(name) {
	Tag = "HydrationStation";

	model = new Model(this, "Assets/Models/HydrationStation.obj");
	model->SetTextureMap(new Texture("Assets/Textures/blue.jpg"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, false);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());
	collider->isTrigger = true;

	transform.position = position;
	UpdateTransform();
}

HydrationStation::~HydrationStation() {

}

void HydrationStation::Update(float deltaTime) {
	UpdateTransform();
}

void HydrationStation::Render(const Camera* camera) {
	model->Render(camera);
}