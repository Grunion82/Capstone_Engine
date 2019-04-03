#include "TextureHandler.h"
#include "../../Core/Systems/Debug.h"
std::unique_ptr<TextureHandler> TextureHandler::textureInstance =  nullptr;
std::map<std::string, UITexture*> TextureHandler::textures = std::map<std::string,UITexture*>();



TextureHandler::TextureHandler()
{
	////Initialize PNG loading
	//int imgFlags = IMG_INIT_PNG;
	//if (!(IMG_Init(imgFlags) & imgFlags))
	//{
	//	printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	//	
	//}
}


TextureHandler::~TextureHandler()
{
	if (textures.size() > 0) {
		for (auto t : textures) {

			glDeleteTextures(sizeof(GLuint), &t.second->textureID);
			delete t.second;

			t.second = nullptr;

		}

		textures.clear();
	}
}

TextureHandler * TextureHandler::GetInstance()
{
	if (textureInstance.get() == nullptr) {

		textureInstance.reset(new TextureHandler);

	}
	return textureInstance.get();
}

void TextureHandler::CreateTexture(const std::string & textureName_, const std::string & textureFilePath_)
{

	UITexture* t = new UITexture();
	SDL_Surface* surface = nullptr;
	try {

		surface = IMG_Load(textureFilePath_.c_str());
		

	}
	catch(std::exception e) {
		Debug::Error("Failed to load texture: " + textureName_, __FILE__, __LINE__);
		std::cout << "failed to load texture" << std::endl;
		return;
	}
	if (surface == nullptr) {
		printf("IMG_Load: %s\n", IMG_GetError());
		Debug::Error("Failed to create texture: " + textureName_, __FILE__, __LINE__);
		return;

	}

	t->width = surface->w;
	t->height = surface->h;

	glGenTextures(1, &t->textureID);
	glBindTexture(GL_TEXTURE_2D, t->textureID);
	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {

		mode = GL_RGBA;

	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S AND T are axis X and Y 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//MITMAP IS ONLY FOR SCALING DOWN
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	textures[textureName_] = t;
	std::cout << "texture added" << std::endl;
	SDL_FreeSurface(surface);
	surface = nullptr;
		
}

const GLuint TextureHandler::GetTexture(const std::string & textureName_)
{
	if (textures.find(textureName_.c_str()) != textures.end()) {

		return textures[textureName_]->textureID;

	}
	return 0;
}

const UITexture * TextureHandler::GetTextureData(const std::string & textureName_)
{
	if (textures.find(textureName_.c_str()) != textures.end()) {

		return textures[textureName_];

	}
	return 0;
}
