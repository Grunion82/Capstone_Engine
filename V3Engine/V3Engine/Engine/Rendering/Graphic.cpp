#include "Graphic.h"
#include <iostream>
#include <SDL.h>
#include "Shader/Shader.h"
#include "2D/Image.h"
#include "../Core/Systems/Window.h"

#include "../Core/Systems/Debug.h"

Quad::Quad()
{
}

Quad::~Quad()
{
	quadShader = nullptr;
	delete quadShader;

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

bool Quad::Init(unsigned int width, unsigned int height)
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
	};

	quadShader = new Shader("quadShader.vs", "quadShader.fs");

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	quadShader->Use();
	quadShader->SetInt("screenTexture", 0);
	quadShader->SetFloat("exposure", 1.0f);

	return true;
}

void Quad::Render()
{
	quadShader->Use();
	glBindVertexArray(quadVAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

bool Quad::Shutdown()
{
	//glDeleteVertexArrays(1, &quadVAO);
	//glDeleteBuffers(1, &quadVBO);

	delete quadShader;
	quadShader = nullptr;

	return true;
}

Graphic::Graphic() {

}

Graphic::~Graphic()
{
}

/*bool Graphic::InitSDL() {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		success = false;
	}

	return success;
}*/

/*bool Graphic::CloseSDL()
{
	SDL_Quit();

	return true;
}*/

bool Graphic::InitOpenGL()
{
	GLenum error = glewInit();

	if (error != GLEW_OK) {
		return false;
	}

	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Name: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	renderingParameter = GL_DEPTH_TEST | GL_STENCIL_TEST;
	clearParameters = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

	glEnable(renderingParameter);

	return true;
}

bool Graphic::Init(Window* w) {
	SCR_WIDTH = w->GetWidth();
	SCR_HEIGHT = w->GetHeight();

	if (!quad.Init(SCR_WIDTH, SCR_HEIGHT) || !gbuffer.Init(SCR_WIDTH, SCR_HEIGHT)) {
		return false;
	}
	
	return true;
}

void Graphic::Update()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(clearParameters);
}

void Graphic::Render()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);

	//glClear(clearParameters);

	//Render quad
	quad.Render();
}

void Graphic::GeometryPass()
{
	gbuffer.BindFramebuffer();
	//Only geometry pass updates depth buffer
	glClear(clearParameters);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	gbuffer.GeometryPass();
}

void Graphic::LightingPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(clearParameters);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	gbuffer.LightPass();
	//gbuffer.LightPass(SCR_WIDTH,SCR_HEIGHT);
}

bool Graphic::Shutdown()
{
	quad.Shutdown();
	gbuffer.Shutdown();

	return true;
}

Graphic* Graphic::instance;
Graphic * Graphic::GetInstance()
{
	if (instance) {
		return instance;
	}
	else {
		instance = new Graphic();
		return instance;
	}
}

bool Graphic::WindowChange(Window* w)
{
	SCR_WIDTH = w->GetWidth();
	SCR_HEIGHT = w->GetHeight();

	gbuffer.WindowChange(SCR_WIDTH, SCR_HEIGHT);

	//For quad
	//quad.WindowChange(SCR_WIDTH, SCR_HEIGHT);

	return true;
}

GBuffer::GBuffer() {

}

GBuffer::~GBuffer() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);

	//glDeleteTextures(GBUFFER_NUM_TEXTURES, textures);

	delete lightShader;
	lightShader = nullptr;

	delete gbufferShader;
	gbufferShader = nullptr;

	//diffuse = nullptr;
	//delete diffuse;
	
	//specular = nullptr;
	//delete specular;

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		delete gbufferTextures[i];
		gbufferTextures[i] = nullptr;
	}
}

bool GBuffer::Init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	//glGenTextures(1, &bufferTexture);
	//glBindTexture(GL_TEXTURE_2D, bufferTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTexture, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	gbufferTextures[GBUFFER_TEXTURE_TYPE_POSITION] = new Texture(width,height,GL_RGB16F,GL_RGB,GL_TEXTURE_2D,GL_FLOAT,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST);
	gbufferTextures[GBUFFER_TEXTURE_TYPE_NORMAL] = new Texture(width, height, GL_RGB16F, GL_RGB, GL_TEXTURE_2D, GL_FLOAT, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	gbufferTextures[GBUFFER_TEXTURE_TYPE_ALBEDOSPEC] = new Texture(width, height,GL_RGBA,GL_RGBA,GL_TEXTURE_2D,GL_UNSIGNED_BYTE,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST);
	
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		gbufferTextures[i]->Use();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gbufferTextures[i]->ID(), 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//Position, normal and albedo(diffuse and specular)
	GLenum drawBuffers[GBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	
	glDrawBuffers(GBUFFER_NUM_TEXTURES, drawBuffers);

	//For depth
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("FAILED TO COMPLETE GBUFFER FBO - GBUFFER::INIT()\n");
		Debug::SetSeverity(MessageType::TYPE_FATAL_ERROR);
		Debug::FatalError("FAILED TO COMPLETE GBUFFER FBO - GBUFFER::INIT()\n", __FILE__, __LINE__);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	gbufferShader = new Shader("gbufferVert.glsl", "gbufferFrag.glsl");
	//gbufferShader->Use();
	//gbufferShader->SetInt("texture_diffuse1", 0);
	//gbufferShader->SetInt("texture_specular1", 1);

	lightShader = new Shader("lightVert.glsl", "lightFrag.glsl");
	lightShader->Use();
	lightShader->SetInt("gPosition", 0);
	lightShader->SetInt("gNormal", 1);
	lightShader->SetInt("gAlbedoSpec", 2);

	return true;
}

void GBuffer::GeometryPass()
{
	gbufferShader->Use();

	//glActiveTexture(GL_TEXTURE0);
	//gbufferTextures[GBUFFER_TEXTURE_TYPE_POSITION]->Use();

	//glActiveTexture(GL_TEXTURE1);
	//gbufferTextures[GBUFFER_TEXTURE_TYPE_ALBEDOSPEC]->Use();
}

void GBuffer::LightPass()
{
	lightShader->Use();

	//Position,normal, then albedospec
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		gbufferTextures[i]->Use();
	}

}

void GBuffer::LightPass(unsigned int width, unsigned int height)
{
	//Disable stencil for directional lights
	//glDisable(GL_STENCIL_TEST

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindForReading();

	for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		//glBindTexture(GL_TEXTURE_2D, textures[i]);
		gbufferTextures[i]->Use();
	}

	unsigned int halfWidth = width / 2;
	unsigned int halfHeight = height / 2;

	SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, width, height,0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, width, height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_ALBEDOSPEC);
	glBlitFramebuffer(0, 0, width, height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	/*SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, w, h, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

}

bool GBuffer::WindowChange(unsigned int width, unsigned int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	gbufferTextures[GBUFFER_TEXTURE_TYPE_POSITION]->Use();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, width);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferTextures[0]->ID(), 0);
	gbufferTextures[GBUFFER_TEXTURE_TYPE_NORMAL]->Use();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, width);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbufferTextures[1]->ID(), 0);
	gbufferTextures[GBUFFER_TEXTURE_TYPE_ALBEDOSPEC]->Use();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, width);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbufferTextures[2]->ID(), 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void GBuffer::BindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

void GBuffer::LightPassBindForReading() {
	glBindFramebuffer(GL_DRAW_BUFFER, 0);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		//glBindTexture(GL_TEXTURE_2D, textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
		gbufferTextures[i]->Use();
	}
}

void GBuffer::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void GBuffer::BindBufferTexture()
{
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, bufferTexture);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE type)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + type);
}

bool GBuffer::Shutdown()
{
	delete gbufferShader;
	gbufferShader = nullptr;
	
	delete lightShader;
	lightShader = nullptr;
	
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		gbufferTextures[i]->Shutdown();
	}

	return true;
}
