#ifndef JEANS_H

#include <Engine/Core/Game/GameObject.h>

class Jeans : public GameObject {
public:
	Jeans();
	~Jeans();

	virtual void Update(float deltaTime) override;
};

#endif // ! JEANS_H