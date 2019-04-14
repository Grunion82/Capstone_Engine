#include "Spikes.h"



Spikes::Spikes() : GameObject("Spikes") {

}

Spikes::Spikes(const std::string& name, glm::vec3 position) : GameObject(name) {
	Tag = "Spikes";

	model = new Model(this, "Assets/Models/Spikes.obj");
	model->SetTextureMap(new Texture("Assets/Textures/steel.jpg"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, false);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert() + glm::vec3(0.0f, 1.0f, 0.0f));
	collider->isTrigger = true;

	transform.position = position;
	UpdateTransform();
}

Spikes::~Spikes() {

}

void Spikes::Update(float deltaTime) {
	UpdateTransform();
}

void Spikes::Render(const Camera* camera) {
	model->Render(camera);
}
