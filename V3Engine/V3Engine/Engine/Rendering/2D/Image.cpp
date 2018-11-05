#include "Image.h"

#include "stb_image.h"

#include <iostream>
#include <glew.h>

Texture::Texture(const char* filepath) : filepath(filepath)
{

}

Cubemap::Cubemap(std::vector<const char*> f) : faces(f)
{
}

Cubemap::~Cubemap()
{
}


Texture::~Texture()
{
}

void Cubemap::Use() {
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemapID);
}

bool Cubemap::Init()
{
	GLenum format;

	int w, h, nr;

	unsigned char* data;

	glGenTextures(1, &cubemapID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	//stbi_set_flip_vertically_on_load(true);

	//Increment faces
	for (GLuint i = 0; i < faces.size(); i++) {
		data = stbi_load(faces[i], &w, &h, &nr, 0);

		switch (nr) {
		case(1):
			format = GL_RED;
			break;
		case(3):
			format = GL_RGB;
			break;
		case(4):
			format = GL_RGBA;
			break;
		}

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "CUBEMAP TEXUTE FAILED TO LOAD AT PATH: " << &faces[i] << std::endl;
			return false;
		}

		stbi_image_free(data);
	}

	//Wrapping
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//R being z-axis
	//Filtering
	//Set to clamp do to hardware limitations
	//Returns edge values when sampling between faces
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}
bool Texture::Init()
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format;

		switch (nrChannels) {
			case(1):
				format = GL_RED;
				break;
			case(3):
				format = GL_RGB;
				break;
			case(4):
				format = GL_RGBA;
				break;
		}

		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);

		//Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE::" << filepath << std::endl;
		return false;
	}

	stbi_image_free(data);

	return true;
}
void Texture::Update()
{
}
void Texture::Render()
{
}
bool Texture::Shutdown()
{
	return true;
}

