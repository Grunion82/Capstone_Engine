#include "V3Engine.h"


std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

V3Engine::V3Engine() {

}


V3Engine::~V3Engine() {

}


V3Engine* V3Engine::GetInstance() {

	if (engineInstance.get() == nullptr)
		engineInstance.reset(new V3Engine);

	return engineInstance.get();
}

void V3Engine::speak() {
	
	printf("Hello from the engine\n");
}