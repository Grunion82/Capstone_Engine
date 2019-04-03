#include "InmediateUI.h"
#include <iostream>
#include"../../V3Engine.h"

InmediateUI::InmediateUI()
{
}

InmediateUI::InmediateUI(SDL_Window * window_, SDL_GLContext context_) : window(window_), context(context_), frame_name("Main_Frame")
{
	//unreachable code
	glsl_version = "#version 330";
	
	if (window == nullptr || context == nullptr) {
		//std::cout << "issue in window or context maybe both" << std::endl;
	}
	
	//Initialize();

	
}

InmediateUI::~InmediateUI()
{
}

void InmediateUI::ShutDown()
{
	Render();
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	delete window;
	window = nullptr;

	delete glsl_version;
	glsl_version = nullptr;

	delete ActualResolution;
	ActualResolution = nullptr;

	delete frame_name;
	frame_name = nullptr;




}



bool InmediateUI::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();//for fonts / io =  input output 
	w = V3Engine::GetInstance()->GetEngineWindow();
	
	//ImGui::StyleColorsDark(); 
	

	glsl_version = "#version 330";
	window = w->GetWindow();
	context = w->GetContext();

	if (window == nullptr) std::cout << "issue in window  " << std::endl;
	if(context == nullptr)std::cout << "issue in context" << std::endl;
	else {/*std::cout << "we good fam" << std::endl;*/ }


	//Initial ui state 
	//UIstate = GameState::Main_Menu;	
	if (font == NULL) {

		font = io.Fonts->AddFontFromFileTTF("Assets/Fonts/BebasNeue-Regular.ttf", 20.0f);

	}
	
	
	// Setup Platform/Renderer bindings //check for proper init
	if (!ImGui_ImplSDL2_InitForOpenGL(window, context)) { return false; }

	if (!ImGui_ImplOpenGL3_Init(glsl_version)) { return false; }


	
	//SDL_PollEvent();takes poll events to sync it later

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(window);
	//ImGui::NewFrame();

	firstTime = true;


	
	return true;
}

bool show_demo_window = true;
bool show_another_window = false;


void InmediateUI::imGuiScene(const char* frame_name_)
{
	
}



void InmediateUI::Render()
{

	ImGui::End();
	// Rendering
	ImGui::Render();
	ImGui::EndFrame();
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//set up an OpenGL context for rendering into an OpenGL window.
	//SDL_GL_MakeCurrent(window, context);

	//default color
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	//clear objects on screen 
	//glClear(GL_COLOR_BUFFER_BIT);
	

	//delete context create a new one basically restart
	//SDL_GL_SwapWindow(window);
	



}
