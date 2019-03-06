#include "V3Engine.h"
#include "Core/Managers/EventManager.h"
#include "Core/Systems/Window.h"
#include "Core/Systems/Input.h"
#include "Core/Systems/Debug.h"
#include "Core/Systems/Timer.h"
#include "Core/Systems/Camera.h"

#include "Rendering/Graphic.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/2D/Image.h"

std::unique_ptr<V3Engine> V3Engine::engineInstance = nullptr;

Timer timer;
Camera* c;

glm::vec3 lightPos[] = { glm::vec3(0.0f,0.0f,10.0f), glm::vec3(0.0f,0.0f,-10.0f),glm::vec3(10.0f,0.0f,0.0f), glm::vec3(-10.0f,0.0f,0.0f) };
glm::vec3 lightColor[] = { glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.2f, 0.8f, 0.2f),glm::vec3(0.2f, 0.2f, 0.8f) };

Shader* shader = nullptr;
Shader* lampShader = nullptr;
Shader* densityCube = nullptr;
Texture* containrTexture = nullptr;

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


V3Engine::V3Engine() : engineWindow(new Window("Leaky Jeans",800,600)) {
	timer.Start();
	engineWindow->Init();
	EventManager::GetInstace()->AddEventSystem<Window>(engineWindow);
	Graphic::GetInstance()->InitOpenGL();
	Input::GetInstance()->Init();
	Graphic::GetInstance()->Init(engineWindow);
	shader = new Shader("genericVert.glsl", "genericFrag.glsl");
	lampShader = new Shader("lampVert.glsl", "lampFrag.glsl");
	densityCube = new Shader("density_volume_vertex.glsl","density_volume_fragment.glsl", "density_volume_geometry.glsl");

	containrTexture = new Texture("container.jpg");
	containrTexture->Init();

	c = new Camera(engineWindow);

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

	while (!Input::GetInstance()->QuitRequested()) {
		timer.UpdateFrameTicks();

		EventManager::GetInstace()->Update();

		if (Input::GetInstance()->WasKeyPressed(SDLK_ESCAPE)) {
			break;
		}
		/*
		if (Input::GetInstance()->IsKeyDown(SDLK_w)) {
			c->Keyboard(CameraMovement::FORWARD, timer.GetDeltaTime());
			//printf("w\n");
		}
		if (Input::GetInstance()->IsKeyDown(SDLK_s)) {
			c->Keyboard(CameraMovement::BACKWARD, timer.GetDeltaTime());
			//printf("s\n");
		}
		if (Input::GetInstance()->IsKeyDown(SDLK_a)) {
			c->Keyboard(CameraMovement::LEFT, timer.GetDeltaTime());
			//printf("a\n");
		}
		if (Input::GetInstance()->IsKeyDown(SDLK_d)) {
			c->Keyboard(CameraMovement::RIGHT, timer.GetDeltaTime());
			//printf("d\n");
		}

		if (Input::GetInstance()->WasKeyPressed(SDLK_p)) {
			engineWindow->Fullscreen();
			Graphic::GetInstance()->WindowChange(engineWindow);
		}*/

		c->Keyboard(Input::GetInstance()->IsKeyDown(SDLK_w) - Input::GetInstance()->IsKeyDown(SDLK_s), Input::GetInstance()->IsKeyDown(SDLK_d) - Input::GetInstance()->IsKeyDown(SDLK_a), timer.GetDeltaTime());

		c->MouseMovement(Input::GetInstance()->GetMouseMotionX(), Input::GetInstance()->GetMouseMotionY(), true);
		
		c->Controller(Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_LEFTY), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTX), Input::GetInstance()->GetJoysticks()[0]->GetAxisDir(SDL_CONTROLLER_AXIS_RIGHTY), timer.GetDeltaTime());
		c->Update();
		
		Render();
	}

	Shutdown();
}

void V3Engine::Render()
{
	//Render
	Graphic::GetInstance()->Update();

	//For geometry
	Graphic::GetInstance()->GeometryPass();

	glm::mat4 model = glm::mat4();
	glm::mat4 view = c->GetViewMatrix();
	glm::mat4 projection = c->GetProjectionMatrix();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f) - c->GetPosition());

	//Graphic::GetInstance()->gbuffer.gbufferShader->Use();
	Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("model", model);
	Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("view", view);
	Graphic::GetInstance()->gbuffer.gbufferShader->SetMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	Graphic::GetInstance()->gbuffer.gbufferShader->SetInt("texture_diffuse1", 0);
	containrTexture->Use();

	glBindVertexArray(cubeTwoVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);

	//For light
	Graphic::GetInstance()->LightingPass();

	//Set light properties
	for (unsigned int i = 0; i < numLights; i++) {
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].position", lightPos[i]);
		Graphic::GetInstance()->gbuffer.lightShader->SetVec3("lights[" + std::to_string(i) + "].color", lightColor[i]);
		float constant = 1.0f;
		float linear = 0.7f;
		float quadratic = 1.8f;
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].linear", linear);
		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);

		float maxBrightness = std::fmaxf(std::fmaxf(lightColor[i].r, lightColor[i].g), lightColor[i].b);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

		Graphic::GetInstance()->gbuffer.lightShader->SetFloat("light[" + std::to_string(i) + "].radius", radius);
	}
	Graphic::GetInstance()->gbuffer.lightShader->SetVec3("viewPos", c->GetPosition());

	//Render to quad
	Graphic::GetInstance()->Render();

	//Copy from gbuffer to default buffer
	Graphic::GetInstance()->gbuffer.BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), 0, 0, engineWindow->GetWidth(), engineWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	for (unsigned int i = 0; i < numLights; i++) {
		model = glm::mat4();
		model = glm::translate(model, lightPos[i] - c->GetPosition());

		lampShader->Use();
		lampShader->SetVec3("lightColor", lightColor[i]);
		lampShader->SetMat4("model", model);
		lampShader->SetMat4("view", view);
		lampShader->SetMat4("projection", projection);

		glBindVertexArray(cubeTwoVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	engineWindow->Render();
}

bool V3Engine::Shutdown()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &cubeTwoVAO);
	glDeleteBuffers(1, &cubeTwoVBO);
	glDeleteVertexArrays(1, &densityVAO);
	glDeleteBuffers(1, &densityVBO);

	EventManager::GetInstace()->Shutdown();
	Graphic::GetInstance()->Shutdown();

	shader = nullptr;
	delete shader;

	lampShader = nullptr;
	delete lampShader;

	densityCube = nullptr;
	delete densityCube;

	containrTexture = nullptr;
	delete containrTexture;

	return true;
}
