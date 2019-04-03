#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H
//	ADL AND GLEW ORDER MATTERS
#include <memory>

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>


struct UITexture {

	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
	float AspectRatio = (width / height);

};


class TextureHandler
{
public:
	static TextureHandler* GetInstance();

	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;
	

	void CreateTexture(const std::string& textureName_, const std::string& textureFilePath_);
	static const GLuint GetTexture(const std::string& textureName_);
	static const UITexture* GetTextureData(const std::string& textureName_);

private:
	TextureHandler();
	~TextureHandler();

	static std::unique_ptr<TextureHandler> textureInstance;
	friend std::default_delete<TextureHandler>;

	static std::map<std::string, UITexture*> textures;

};

#endif //TEXTUREHANDLER_H

