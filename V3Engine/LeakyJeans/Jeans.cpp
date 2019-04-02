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
	if (Input::GetInstance()->WasKeyPressed(SDLK_RIGHT))
		rigidBody->ApplyForce(glm::vec3(5.0f, 0.0f, 0.0f));

	rigidBody->ApplyForce(glm::vec3(Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a),0.0f,-(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s))));

	UpdateTransform();

	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Update(deltaTime);
			}
		}
	}
}

void Jeans::Render(const Camera* camera) {
	model->Render(camera);
	if (childObjects.size() > 0) {
		for (unsigned int i = 0; i < childObjects.size(); i++) {
			if (childObjects[i]) {
				childObjects[i]->Render(camera);
			}
		}
	}
}

void Jeans::CollisionEnter(GameObject* collisionObj) {
	if (collisionObj->GetTag() == "Platform")
		hasJumped = false;
}

void Jeans::CollisionStay(GameObject* collisionObj) {

}

void Jeans::CollisionExit(GameObject * collisionObj) {

}
