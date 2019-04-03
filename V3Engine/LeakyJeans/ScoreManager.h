#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <memory>

class ScoreManager {
private:
	ScoreManager();
	~ScoreManager();

	static float timeElapsed;

	static std::unique_ptr<ScoreManager> scoreManagerInstance;
	friend std::default_delete<ScoreManager>;

public:
	//Standard 4 delete lines
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager(ScoreManager&&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;
	ScoreManager& operator=(ScoreManager&&) = delete;

	static ScoreManager* GetInstance();
	static float GetTimeElapsed() { return timeElapsed;  }

	void Update(float deltaTime);
	float CalculateScore(float wetnessValue);
};

#endif // !SCOREMANAGER_H