#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <glew.h>
#include <gl/GL.h>

class Window;
class Shader;

class Graphic
{
	unsigned int FBO;
	unsigned int bufferTexture;
	unsigned int RBO;
	unsigned int quadVAO, quadVBO;
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	int renderingParameter;
	int clearParameters;

	Shader* quadShader;
	
public:
	Graphic();
	~Graphic();

	bool InitOpenGL(Window* w);
	bool InitOpenGL(unsigned int width, unsigned int height);

	//Call this before any graphics rendering
	void Update();
	void Render();
	bool WindowChange(Window* w);
	bool WindowChange(unsigned int width, unsigned int height);
};

#endif // !GRAPHIC_H
