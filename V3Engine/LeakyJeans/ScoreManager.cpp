#include "ScoreManager.h"

std::unique_ptr<ScoreManager> ScoreManager::scoreManagerInstance = nullptr;
float ScoreManager::timeElapsed = 0.0f;

ScoreManager::ScoreManager() {

}


ScoreManager::~ScoreManager() {

}

ScoreManager* ScoreManager::GetInstance() {
	if (scoreManagerInstance == nullptr) {
		scoreManagerInstance.reset(new ScoreManager());
	}

	return scoreManagerInstance.get();
}

void ScoreManager::Update(float deltaTime) {
	timeElapsed += deltaTime;
}

float ScoreManager::CalculateScore(float wetnessValue) {
	float totalScore = 0.0f;

	float wetnessScore = wetnessValue * 0.65f;
	float timeScore = 100.0f - (timeElapsed * 0.5);

	totalScore = wetnessScore + timeScore;
	return totalScore;
}
