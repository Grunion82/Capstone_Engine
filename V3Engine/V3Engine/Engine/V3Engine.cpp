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
	c[1]->Translate(glm::vec3(0.0f, 0.0f, 15.0f));

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
	c = new Camera(engineWindow);
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

		c[0]->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer.GetDeltaTime());

		c[0]->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);
		
		//c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer.GetDeltaTime());
		//c2->Controller(Input::GetInstance()->GetJoysticks()[1]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[1]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[1]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[1]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer.GetDeltaTime());

		c[0]->Update();
		c[1]->Update();
		c->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer->GetDeltaTime());

		c->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);

		//c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer->GetDeltaTime());
		c->Update();

		Render();
		SDL_Delay(timer->GetSleepTime(90)); // 90 fps
	}

	Shutdown();
}

void V3Engine::Render() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Render
	//Graphic::GetInstance()->Update();

<<<<<<< HEAD
	for (unsigned int i = 0; i < c.size(); i++) {
		//For geometry
		Graphic::GetInstance()->GeometryPass();

		if (i == 0) {
			glViewport(0, engineWindow->GetHeight() / 2, engineWindow->GetWidth(), engineWindow->GetHeight() / 2);
			//SDL_Rect topViewport;
			//topViewport.x = 0;
			//topViewport.y = 0;
			//topViewport.w = engineWindow->GetWidth();
			//topViewport.h = engineWindow->GetHeight();
			//SDL_RenderSetViewport(engineWindow->GetRenderer(), &topViewport);
		}
		else {
			glViewport(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight() / 2);
			//SDL_Rect bottomViewport;
			//bottomViewport.x = 0;
			//bottomViewport.y = engineWindow->GetHeight()/2;
			//bottomViewport.w = engineWindow->GetWidth();
			//bottomViewport.h = engineWindow->GetHeight();
			//SDL_RenderSetViewport(engineWindow->GetRenderer(), &bottomViewport);
		}

		glm::mat4 model = glm::mat4();
		glm::mat4 view = c[i]->GetViewMatrix();
		glm::mat4 projection = c[i]->GetProjectionMatrix();

		//"Cube"
		model = glm::translate(model, glm::vec3(0,0,-3.0f) - c[i]->GetPosition());

		//Graphic::GetInstance()->gbuffer.gbufferShader->Use();
		Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("model", model);
		Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("view", view);
		Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("projection", projection);

		glBindVertexArray(cubeTwoVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//For light
		Graphic::GetInstance()->LightingPass();

		//Set light properties
		for (unsigned int j = 0; j < numLights; j++) {
			Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(j) + "].position", lightPos[j]);
			Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(j) + "].color", lightColor[j]);
			float constant = 1.0f;
			float linear = 0.7f;
			float quadratic = 1.8f;
			Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(j) + "].linear", linear);
			Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(j) + "].quadratic", quadratic);
	//For geometry
	//Graphic::GetInstance()->GeometryPass();

	//For light
	//Graphic::GetInstance()->LightingPass();

	//Set light properties
	/*for (unsigned int i = 0; i < numLights; i++) {
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].position", lightPos[i]);
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].color", lightColor[i]);
		float constant = 1.0f;
		float linear = 0.7f;
		float quadratic = 1.8f;
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].linear", linear);
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
>>>>>>> origin/grunion

			float maxBrightness = std::fmaxf(std::fmaxf(lightColor[j].r, lightColor[j].g), lightColor[j].b);
			float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

			Graphic::GetInstance()->gbuffer.lightShader->SetFloat("light[" + std::to_string(j) + "].radius", radius);
		}
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("viewPos", c[i]->GetPosition());
	}
	//glViewport(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight());
	//Render to quad
	Graphic::GetInstance()->Render();

	//Copy from gbuffer to default buffer
	Graphic::GetInstance()->gbuffer.BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), 0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);*/

	for (unsigned int i = 0; i < c.size(); i++) {
		for (unsigned int j = 0; j < numLights; j++) {
			if (i == 0) {
				glViewport(0, engineWindow->GetHeight() / 2, engineWindow->GetWidth(), engineWindow->GetHeight() / 2);
				//SDL_Rect topViewport;
				//topViewport.x = 0;
				//topViewport.y = 0;
				//topViewport.w = engineWindow->GetWidth();
				//topViewport.h = engineWindow->GetHeight();
				//SDL_RenderSetViewport(engineWindow->GetRenderer(), &topViewport);
			}
			else {
				glViewport(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight() / 2);
				//SDL_Rect bottomViewport;
				//bottomViewport.x = 0;
				//bottomViewport.y = engineWindow->GetHeight()/2;
				//bottomViewport.w = engineWindow->GetWidth();
				//bottomViewport.h = engineWindow->GetHeight();
				//SDL_RenderSetViewport(engineWindow->GetRenderer(), &bottomViewport);
			}

			glm::mat4 model = glm::mat4();
			glm::mat4 view = c[i]->GetViewMatrix();
			glm::mat4 projection = c[i]->GetProjectionMatrix();

			model = glm::translate(model, lightPos[j] - c[i]->GetPosition());

			lampShader->Use();
			lampShader->SetVec3("lightColor", lightColor[j]);
			lampShader->SetMat4("model", model);
			lampShader->SetMat4("view", view);
			lampShader->SetMat4("projection", projection);

			glBindVertexArray(cubeTwoVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}
=======
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	currentGame->Render(c);
>>>>>>> origin/grunion

	//glViewport(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight());

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
