#include "Jeans.h"

#include <stdio.h>
#include <Engine/Core/Systems/Camera.h>
#include <Engine/Core/Systems/Input.h>

#include "ScoreManager.h"

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

	speed = MAX_SPEED;
	drainRatio = 1.0f;
	waterLevel = MAX_WATER_LEVEL;

	model = new Model(this, "Assets/Models/jeans.obj");
	model->SetTextureMap(new Texture("Assets/Textures/jeansbasecolor.png"));
	model->SetShader(new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl"));

	rigidBody = new RigidBody(this, true);

	collider = new BoundingBox(this, model->GetMinVert(), model->GetMaxVert());

	startPos = transform.position = position;
	UpdateTransform();
}

Jeans::~Jeans() {

}

void Jeans::MoveJeans(glm::vec3 displacement, float time) {
	if (displacement.length() >= 1.0f) {
		Translate(displacement * speed * time);
		waterLevel -= drainRatio * time;
	}
}

void Jeans::Update(float deltaTime) {

	if (ScoreManager::GetTimeElapsed() > 4.0f)
		rigidBody->isEnabled = true;

	waterLevel = glm::clamp(waterLevel, 0.0f, MAX_WATER_LEVEL);
	speed = MAX_SPEED - (waterLevel / 10.0f);
	
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
	if (collisionObj->GetTag() == "Spikes") {
		drainRatio += 5.0f;
	}
	if (collisionObj->GetTag() == "Flag") {
		printf("final score: %f\n", ScoreManager::GetInstance()->CalculateScore(waterLevel));
	}
	if (collisionObj->GetTag() == "DeathBox") {
		printf("hit box\n");
		rigidBody->velocity = glm::vec3(0.0f);
		Translate(startPos - transform.position);
		UpdateTransform();
	}
}

void Jeans::CollisionStay(GameObject* collisionObj) {
	if (collisionObj->GetTag() == "HydrationStation") {
		waterLevel += 0.3f;
	}
}

void Jeans::CollisionExit(GameObject* collisionObj) {
	if (collisionObj->GetTag() == "Platform")
		hasJumped = true;
}

int Jeans::GetWaterLevel() const {
	return waterLevel;
}
