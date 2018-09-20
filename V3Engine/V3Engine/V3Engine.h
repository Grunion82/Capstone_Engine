#ifndef V3ENGINE_H
#define V3ENGING_H

#include <memory>
#include <stdio.h>
#include <SDL.h>

class V3Engine {
private:
	V3Engine();
	~V3Engine();

public:
	
	V3Engine(const V3Engine&) = delete;
	V3Engine(V3Engine&&) = delete;
	V3Engine& operator=(const V3Engine&) = delete;
	V3Engine& operator=(V3Engine&&) = delete;

	static std::unique_ptr<V3Engine> engineInstance;
	friend std::default_delete<V3Engine>;

	static V3Engine* GetInstance();
	void speak();
};

#endif // ! V3ENGINE_H