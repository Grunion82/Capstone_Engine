#ifndef HYDRATIONSTATION_H
#define HYDRATIONSTATION_H

#include <Engine/Core/Game/GameObject.h>

class HydrationStation : public GameObject {
private:


public:
	HydrationStation();
	HydrationStation(const std::string& name, glm::vec3 position);
	~HydrationStation();

	void Update(float deltaTime) override;
	void Render(const class Camera* camera) override;
};

#endif // !HYDRATIONSTATION_H