#include "V3Engine.h"

#include <SDL.h>
#include "Core/Managers/EventManager.h"
#include "Core/Systems/Window.h"
#include "Core/Systems/Input.h"
#include "Core/Systems/Debug.h"
#include "Core/Systems/Timer.h"
#include "Core/Systems/Camera.h"
#include "Core/Game/GameInterface.h"

#include "Math/PhysicsManager.h"

#include "Rendering/Graphic.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/2D/Image.h"

std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

std::vector<Camera*> c;

Timer timer;

glm::vec3 lightPos[] = { glm::vec3(0.0f,0.0f,10.0f), glm::vec3(0.0f,0.0f,-10.0f),glm::vec3(10.0f,0.0f,0.0f), glm::vec3(-10.0f,0.0f,0.0f) };
glm::vec3 lightColor[] = { glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.2f, 0.8f, 0.2f),glm::vec3(0.2f, 0.2f, 0.8f) };

Shader* shader = nullptr;
Shader* lampShader = nullptr;
Shader* densityCube = nullptr;

//For a cube with normals
float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
//For a cube with normals and texture coordinates
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
float quadVertices[] = { 
// positions   // texCoords
-1.0f,  1.0f,  0.0f, 1.0f,
-1.0f, -1.0f,  0.0f, 0.0f,
1.0f, -1.0f,  1.0f, 0.0f,

-1.0f,  1.0f,  0.0f, 1.0f,
1.0f, -1.0f,  1.0f, 0.0f,
1.0f,  1.0f,  1.0f, 1.0f
};

unsigned int numLights = 4;

unsigned int cubeVAO, cubeVBO, densityVAO;
unsigned int cubeTwoVAO, cubeTwoVBO, densityVBO;

V3Engine::V3Engine() : engineWindow(new Window("Leaky Jeans",1024,768)) {
	Debug::DebugInit();
	engineWindow->Init();
	timer = new Timer();
	timer->Start();
	EventManager::GetInstace()->AddEventSystem<Window>(engineWindow);
	Graphic::GetInstance()->InitOpenGL();
	Input::GetInstance()->Init();
	Graphic::GetInstance()->Init(engineWindow);

	c.reserve(2);
	c.push_back(new Camera(engineWindow));
	c.push_back(new Camera(engineWindow));
	c[0]->SetCameraViewport(0, engineWindow->GetHeight() / 2, engineWindow->GetWidth(), engineWindow->GetHeight()/2);
	c[1]->Translate(glm::vec3(0.0f, 0.0f, 15.0f));
	c[1]->SetCameraViewport(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight()/2);


	//Cube 
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);//Texture
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &cubeTwoVAO);
	glGenBuffers(1, &cubeTwoVBO);
	glBindVertexArray(cubeTwoVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTwoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);//Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	//Plane for density
	/*
	glGenVertexArrays(1, &densityVAO);
	glGenBuffers(1, &densityVBO);
	glBindVertexArray(densityVAO);
	glBindBuffer(GL_ARRAY_BUFFER, densityVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	*/
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
}

void V3Engine::NewGame(GameInterface* game) {
	currentGame = game;
}

void V3Engine::Run() {
	while (!Input::GetInstance()->QuitRequested()) {
		timer->UpdateFrameTicks();

		EventManager::GetInstace()->Update();
		currentGame->Update(timer->GetDeltaTime());
		PhysicsManager::GetInstance()->Update(timer->GetDeltaTime());

		if (Input::GetInstance()->WasKeyPressed(SDLK_ESCAPE)) {
			break;
		}

		c[0]->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		c[0]->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);
		
		c[0]->Controller(Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(0,SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());
		c[1]->Controller(Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoystickAxisDir(1,SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());

		for (unsigned int i = 0; i < c.size(); i++) {
			c[i]->Update();
		}
		//c->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		//c->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);

		//c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());
		//c->Update();

		Render();
		//SDL_Delay(timer->GetSleepTime(90)); // 90 fps
	}

	Shutdown();
}

void V3Engine::Render() {
	Graphic::GetInstance()->Update();
	//Graphic::GetInstance()->GeometryPass();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	for (unsigned int i = 0; i < c.size(); i++) {
		c[i]->Render();
		currentGame->Render(c[i]);
	}

	//Graphic::GetInstance()->Render();
	//Graphic::GetInstance()->gbuffer.BindForReading();
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), 0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	engineWindow->Render();
}

bool V3Engine::Shutdown() {
	EventManager::GetInstace()->Shutdown();
	Graphic::GetInstance()->Shutdown();

	delete currentGame;
	currentGame = nullptr;

	delete timer;
	timer = nullptr;

	delete engineWindow;
	engineWindow = nullptr;

	return true;
}