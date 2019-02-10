#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION

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
	Texture(unsigned int width, unsigned int height, int format = 0x1907, int format2 = 0x1907, int tex = 0x0DE1, int type = 0x1406, int styleS = 0x2901, int styleT = 0x2901, int fminStyle = 0x2601, int fmaxStyle = 0x2601);
	~Texture();

	void Use(unsigned int type = 0x0DE1);

	inline unsigned int ID() { return textureID; }
	bool Init();
	void Update();
	void Render();
	bool Shutdown();
};

#endif // !IMAGE_H
