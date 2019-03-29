#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <glew.h>
#include <gl/GL.h>

class Window;
class Shader;
class Texture;

class Quad
{
	Shader* quadShader = nullptr;
	unsigned int quadVAO, quadVBO;
public:
	Quad();
	~Quad();

	bool Init(unsigned int w, unsigned int h);

	//Call this before any graphics rendering
	//void Update();
	void Render();
	bool Shutdown();
};


class GBuffer {

public:
	Shader* gbufferShader = nullptr;
	Shader* lightShader = nullptr;

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_ALBEDOSPEC,
		//GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	void BindForWriting();
	void BindForReading();
	void LightPassBindForReading();
	void BindFramebuffer();
	void BindBufferTexture();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE type);
	bool WindowChange(unsigned int width, unsigned int height);

	bool Init(unsigned int width,unsigned int height);
	void LightPass(unsigned int width,unsigned int height);
	void LightPass();
	void GeometryPass();
	bool Shutdown();
private:
	//Textures for position, normal, and albedo specular
	Texture* gbufferTextures[GBUFFER_NUM_TEXTURES];
	//These two are suppose to be the textures that are sampled from
	unsigned int FBO;
	//unsigned int bufferTexture;

	//Albedo,normals,position
	//unsigned int textures[GBUFFER_NUM_TEXTURES];
	unsigned int RBO;
};

class Graphic {
	static Graphic* instance;
public:
	Quad quad;
	GBuffer gbuffer;

private:
	int renderingParameter;
	int clearParameters;

	unsigned int SCR_WIDTH, SCR_HEIGHT;

	Graphic();
	~Graphic();
	//bool InitSDL();
	//bool CloseSDL();

public:
	static Graphic* GetInstance();

	bool InitOpenGL();
	bool Init(Window* w);
	bool WindowChange(Window* w);

	void Update();
	void Render();
	//Call this before rendering geomtry/color based
	void GeometryPass();
	//Call this before rendering lighting
	void LightingPass();
	bool Shutdown();
};
#endif // !GRAPHIC_H
