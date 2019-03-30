#include "Platform.h"



Platform::Platform() : GameObject("Platform") {

}

Platform::Platform(const std::string& name, glm::vec3 position) : GameObject(name) {
	Tag = "Platform";
	transform.position = position;

	model = new Model(this, "Assets/Models/Platform.obj");
	model->SetTextureMap(new Texture("Assets/Textures/grass.jpg"));
	model->SetShaderProgram(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this);
	rigidBody->isEnabled = false;
	rigidBody->mass = 100.0f;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());

	UpdateTransform();
}

Platform::~Platform() {

}

void Platform::Update(float deltaTime) {
	
	UpdateTransform();
	model->Update(deltaTime);
	collider->Update(deltaTime);
}

void Platform::Render(const Camera* camera) {
	model->Render(camera);
}
