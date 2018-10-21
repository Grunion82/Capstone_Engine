#include "V3Engine.h"
#include "Core/Systems/Window.h"
#include "Core/Systems/Input.h"
#include "Core/Systems/Debug.h"

std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

V3Engine::V3Engine() : engineInput(new Input()),engineWindow(new Window("Leaky Jeans",800,600)) {
	engineWindow->Init();
	engineInput->Init();
}


V3Engine::~V3Engine() {

}


V3Engine* V3Engine::GetInstance() {

	if (engineInstance.get() == nullptr)
		engineInstance.reset(new V3Engine);

	return engineInstance.get();
}

void V3Engine::speak() {
	
	printf("Engine speaketh\n");

	while (!engineInput->QuitRequested()) {
		engineWindow->Update();
		engineWindow->Render();
		engineInput->Update();
		if(engineInput->GetJoystick(0)->WasJoyStickButtonPressed(0))
		std::cout << engineInput->GetJoystick(0)->WasJoyStickButtonPressed(0) << std::endl;
	}
	engineInput->Shutdown();
	engineWindow->Shutdown();
}