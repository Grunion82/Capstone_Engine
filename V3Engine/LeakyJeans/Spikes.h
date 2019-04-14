#ifndef SPIKES_H
#define SPIKES_H

#include <Engine/Core/Game/GameObject.h>

class Spikes : public GameObject {
private:

public:
	Spikes();
	Spikes(const std::string& name, glm::vec3 position);
	~Spikes();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;
};

#endif // !SPIKES_H