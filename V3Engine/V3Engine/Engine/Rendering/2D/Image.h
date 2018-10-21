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
};

class Texture
{
	unsigned int textureID;

	int width, height, nrChannels;

	const char* filepath;
public:
	Texture(const char* filepath);
	~Texture();

	void Use();

	inline unsigned int ID() { return textureID; }
	bool Init();
	void Update();
	void Render();
	bool Shutdown();
};

#endif // !IMAGE_H
