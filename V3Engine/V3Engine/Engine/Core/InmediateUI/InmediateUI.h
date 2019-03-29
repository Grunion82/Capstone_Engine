#pragma once
#ifndef INMEDIATEUI_H
#define INMEDIATEUI_H
//texture
#include "../../Rendering/2D/Image.h"
#include <SDL_opengl.h>
#include <SDL.h>
#include<glm\glm.hpp>
#include <vector>
//imgui 
#include "../../../Resources/Libraries/imgui/imgui.h"
#include "../../../Resources/Libraries/imgui/imgui_impl_sdl.h"
#include "../../../Resources/Libraries/imgui/imgui_impl_opengl3.h"





//Game scene or state //change this to UIstate
enum class GameState :unsigned __int8 {

	Main_Menu,
	Options_Menu,
	In_Game

};


class InmediateUI
{
public:


	InmediateUI();
	virtual ~InmediateUI();
	InmediateUI(SDL_Window * window_, SDL_GLContext context_);
	 bool Initialize(SDL_Window * window_, SDL_GLContext context_);
	 void Render();
	 void imGuiScene(const char* frame_name_);
	 
	virtual void Update(const float deltaTime_) = 0;
	virtual void ShutDown();
	
	bool firstTime;

	//access to the engine ui state
	GameState UIstate;
	//ImguiIO
	
	SDL_Window *window;
	ImGuiIO io;
private:

	ImFont* font;
	const char* glsl_version;
	const char* frame_name;
	SDL_GLContext context;


};
#endif // !INMEDIATEUI_H
