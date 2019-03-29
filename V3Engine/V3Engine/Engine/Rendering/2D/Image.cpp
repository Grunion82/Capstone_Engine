#include "Image.h"

#include "stb_image.h"
#include "../../Core/Systems/Debug.h"

#include <glew.h>


Cubemap::Cubemap(std::vector<const char*> f) : faces(f) {
	Init();
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &cubemapID);
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

	stbi_set_flip_vertically_on_load(false);

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
			std::cout << "CUBEMAP TEXTURE FAILED TO LOAD AT PATH: " << faces[i] << std::endl;
			std::string message = "CUBEMAP TEXTURE FAILED TO LOAD AT PATH: ";
			message += faces[i];
			Debug::SetSeverity(MessageType::TYPE_WARNING);
			Debug::Warning(message, __FILE__, __LINE__);
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

Texture::Texture(const char* filepath) : filepath(filepath)
{
	if (filepath != nullptr) {
		Init();
	}
}

Texture::Texture(unsigned int width,unsigned int height, int format, int format2, int tex, int type,int styleS, int styleT, int fminStyle, int fmaxStyle) : width(width),height(height)
{
	glGenTextures(1, &textureID);
	glBindTexture(tex,textureID);

	glTexImage2D(tex, 0, format, width, height, 0, format2, type, NULL);
	
	//Wrapping
	glTexParameteri(tex, GL_TEXTURE_WRAP_S, styleS);
	glTexParameteri(tex, GL_TEXTURE_WRAP_T, styleT);

	//Filtering
	glTexParameteri(tex, GL_TEXTURE_MIN_FILTER, fminStyle);
	glTexParameteri(tex, GL_TEXTURE_MAG_FILTER, fmaxStyle);

	glBindTexture(tex, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

//Bind the texture
void Texture::Use(unsigned int type)
{
	glBindTexture(type, textureID);
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
		std::cout << "FAILED TO LOAD TEXTURE: " << filepath << std::endl;
		std::string message = "FAILED TO LOAD TEXTURE: ";
		message += filepath;
		Debug::SetSeverity(MessageType::TYPE_WARNING);
		Debug::Warning(message, __FILE__, __LINE__);
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
	glDeleteTextures(1, &textureID);

	return true;
}

