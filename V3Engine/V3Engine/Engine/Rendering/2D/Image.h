#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION

#include <glew.h>
#include <vector>

class Cubemap {
	std::vector<const char*> faces;
	unsigned int cubemapID;
public:
	Cubemap(std::vector<const char*> f);
	~Cubemap();

	void Use();
	inline unsigned int ID() { return cubemapID; }
	bool Init();
};

class Texture
{
	unsigned int textureID;

	int width, height, nrChannels;

	const char* filepath;
public:
	Texture(const char* filepath);
	//For a specification of a texture
	Texture(unsigned int width, unsigned int height, int format = GL_RGB, int format2 = GL_RGB, int tex = GL_TEXTURE_2D, int type = GL_FLOAT, int styleS = GL_REPEAT, int styleT = GL_REPEAT, int fminStyle = GL_LINEAR, int fmaxStyle = GL_LINEAR);
	~Texture();

	void Use(unsigned int type = GL_TEXTURE_2D);

	inline unsigned int ID() { return textureID; }
	bool Init();
	void Update();
	void Render();
	bool Shutdown();
};

#endif // !IMAGE_H
