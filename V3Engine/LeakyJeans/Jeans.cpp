#include "Jeans.h"

#include <stdio.h>
#include <Engine/Core/Systems/Camera.h>
#include <Engine/Core/Systems/Input.h>

Jeans::Jeans() : GameObject("Jeans") {
	model = new Model(this, "Assets/Models/cube.obj");
	model->SetTextureMap(new Texture("Assets/Textures/jeans.jpg"));
	model->SetShaderProgram(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMinVert());

	transform.position = glm::vec3(0.0f, 2.0f, -5.0f);
	UpdateTransform();
}

Jeans::Jeans(const std::string& name, glm::vec3 position) : GameObject(name) {
	Tag = "Player";
	hasJumped = false;

	model = new Model(this, "Assets/Models/cube.obj");
	model->SetTextureMap(new Texture("Assets/Textures/jeans.jpg"));
	model->SetShaderProgram(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, true);
	rigidBody->SetBounciness(0.5f);

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());

	transform.position = position;
	UpdateTransform();
}

Jeans::~Jeans() {

}

void Jeans::Update(float deltaTime) {
	//transform.angle += 1.0f * deltaTime;
	if (Input::GetInstance()->WasKeyPressed(SDLK_SPACE) && !hasJumped) {
		rigidBody->ApplyForce(glm::vec3(0.0f, 100.0f, 0.0f));
		hasJumped = true;
	}
	


	UpdateTransform();
	model->Update(deltaTime);
	collider->Update(deltaTime);
}

void Jeans::Render(const Camera* camera) {
	model->Render(camera);
}
