#include "Jeans.h"

#include <stdio.h>
#include <Engine/Core/Systems/Camera.h>
#include <Engine/Core/Systems/Input.h>

Jeans::Jeans() : GameObject("Jeans") {
	model = new Model(this, "Assets/Models/jeans.obj");
	model->SetTextureMap(new Texture("Assets/Textures/jeansbasecolor.png"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));
	//model->SetShader(Graphic::GetInstance()->gbuffer.gbufferShader);

	rigidBody = new RigidBody(this);
	rigidBody->isEnabled = false;

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMinVert());

	transform.position = glm::vec3(0.0f, 10.0f, -5.0f);
	UpdateTransform();
}

Jeans::Jeans(const std::string& name, glm::vec3 position) : GameObject(name) {
	Tag = "Player";
	hasJumped = false;
	speed = 10.0f;

	model = new Model(this, "Assets/Models/jeans.obj");
	model->SetTextureMap(new Texture("Assets/Textures/jeansbasecolor.png"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, true);
	//rigidBody->SetBounciness(0.5f);

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());

	transform.position = position;
	UpdateTransform();
}

Jeans::~Jeans() {

}

void Jeans::Update(float deltaTime) {
	//transform.angle += 1.0f * deltaTime;
	if (Input::GetInstance()->WasKeyPressed(SDLK_SPACE) && !hasJumped) {
		rigidBody->ApplyForce(glm::vec3(0.0f, 125.0f, 0.0f));
		hasJumped = true;
	}
	if (Input::GetInstance()->IsKeyDown(SDLK_RIGHT))
		Translate(glm::vec3(speed * deltaTime, 0.0f, 0.0f));
	if (Input::GetInstance()->IsKeyDown(SDLK_LEFT))
		Translate(glm::vec3(-speed * deltaTime, 0.0f, 0.0f));
	if (Input::GetInstance()->IsKeyDown(SDLK_UP))
		Translate(glm::vec3(0.0f, 0.0f, -speed * deltaTime));
	if (Input::GetInstance()->IsKeyDown(SDLK_DOWN))
		Translate(glm::vec3(0.0f, 0.0f, speed * deltaTime));

	UpdateTransform();
}

void Jeans::Render(const Camera* camera) {
	model->Render(camera);
}

void Jeans::CollisionEnter(GameObject* collisionObj) {
	if (collisionObj->GetTag() == "Platform")
		hasJumped = false;
}

void Jeans::CollisionStay(GameObject* collisionObj) {
	//printf("stay\n");
}

void Jeans::CollisionExit(GameObject* collisionObj) {
	//printf("exit\n");
	if (collisionObj->GetTag() == "Platform")
		hasJumped = true;
}
