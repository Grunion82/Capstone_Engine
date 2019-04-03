#include "Flag.h"



Flag::Flag() : GameObject("Flag") {

}

Flag::Flag(const std::string & name, glm::vec3 position) : GameObject(name) {
	Tag = "Flag";

	model = new Model(this, "Assets/Models/Flag.obj");
	model->SetTextureMap(new Texture("Assets/Textures/steel.jpg"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, false);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());
	collider->isTrigger = true;

	transform.position = position;
	UpdateTransform();
}


Flag::~Flag() {

}

void Flag::Update(float deltaTime) {
	UpdateTransform();
}

void Flag::Render(const Camera* camera) {
	model->Render(camera);
}
