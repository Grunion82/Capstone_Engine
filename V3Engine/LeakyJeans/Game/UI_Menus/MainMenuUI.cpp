#include "MainMenuUI.h"
#include <array>
#include <iostream>
#include<Engine/V3Engine.h>

#include "../../../V3Engine/Engine/Core/Game/GameInterface.h"


MainMenuUI::MainMenuUI(GameInterface* ref)
{
	gameRef = ref;
}


MainMenuUI::~MainMenuUI()
{
	//clean up 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	gameRef = nullptr;

}

//Flags for the window
static bool no_resize = true;
static bool no_move = true;//this might cause issues when the window change w and h 
static bool no_collapse = true;
static bool auto_resize = true;
static bool no_menu = true;
static bool no_scrollbar = true;
static bool no_titlebar = true;

void MainMenuUI::imGuiScene(const char * frame_name_)
{
	static float f = 0.0f;
	static int counter = 10;
	static bool showMouse = true;
	
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		
		
		
		
		//UI FOR EDITIING
		static float ue_slider = 0.0;
		static float ue_Slider = 0.0f;
		static bool ue_Bool = false;
		static bool ue_Bool_2 = false;
		static bool ue_Bool_3 = false;
		static bool ue_Bool_4 = false;

		ImGui::SliderFloat("", & ue_Slider, 0.0f, 800.0f);
		ImGui::SliderFloat("##2", &ue_slider, 0.0f, 600.0f);
		ImGui::Checkbox("mybool", &ue_Bool);
		ImGui::Checkbox("mybool_1", &ue_Bool_2);
		ImGui::Checkbox("mybool_2", &ue_Bool_3);
		ImGui::Checkbox("mybool_3", &ue_Bool_4);

		
		
	//setting the size and position of our window//1280
	ImVec2 defaultSize = ImVec2(1024, 576);
	
	ImVec2 size = ImVec2(w->GetCurrentResolution().w ,w->GetCurrentResolution().h);//window size later update this with the sidth and height of the window
	if (size.x == 0 || size.y == 0) {
		size = defaultSize;
	}
	if (ue_Bool_2) {
		size.x = 1024;
		size.y = 576;
		ue_Bool_3 = false;
		ue_Bool_4 = false;
	}
	if (ue_Bool_3) {

		size.x = 1152;
		size.y = 648;
		ue_Bool_2 = false;
		ue_Bool_4 = false;

	}
	if (ue_Bool_4) {

		size.x = 1920;
		size.y = 1040;
		ue_Bool_3 = false;
		ue_Bool_2 = false;


	}
	//calculate aspect ration in case display changes; 
	//calculates the windows change so we can relate that number with all the elements exposed in the UI/// NOTE: I CAN CREATE AN ENUM WITH ALL THE RESOLUTIONS
	float AspectRatio = CalculateDisplayChange(defaultSize.x, size.x);

	
	ImVec2 buttonSize = ImVec2(150.0f * AspectRatio, 50 * AspectRatio);
	ImVec2 pos = ImVec2(0, 0);
	ImVec2 widgetPos = ImVec2(0, 0);


	//window flags
	ImGuiWindowFlags window_flags = 0;

	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)			window_flags |= ImGuiWindowFlags_NoResize;

	if (auto_resize)		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_titlebar)		window_flags |= ImGuiWindowFlags_NoTitleBar;

	//size and position of the window
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(pos);

	//creates a window
	ImGui::Begin("Main Menu", false, window_flags); 
	WindowStyle(&ImGui::GetStyle());

	//Textures
	ImTextureID my_LeakyLogo = (void*)(size_t)TextureHandler::GetTexture("LeakyLogo");

	ImTextureID my_background = (void*)(size_t)TextureHandler::GetTexture("Background_4");
	ImTextureID my_EngineLogo = (void*)(size_t)TextureHandler::GetTexture("EngineLogo");

	//2D Sprites
	ImTextureID Anim_Dancing = (void*)(size_t)TextureHandler::GetTexture("DancingAnim");
	ImTextureID Anim_kick = (void*)(size_t)TextureHandler::GetTexture("KickAnim");
	ImTextureID Anim_Jump = (void*)(size_t)TextureHandler::GetTexture("JumpAnim");
	ImTextureID Anim_Wiggle = (void*)(size_t)TextureHandler::GetTexture("WiggleAnim");

	float my_file_w = TextureHandler::GetTextureData("LeakyLogo")->width * 0.6 * AspectRatio;
	float my_file_h = TextureHandler::GetTextureData("LeakyLogo")->height * 0.6  * AspectRatio;

	float my_EngineLogo_w = TextureHandler::GetTextureData("EngineLogo")->width/ 5 * (AspectRatio - 0.1);
	float my_EngineLogo_h = TextureHandler::GetTextureData("EngineLogo")->height/ 5 * (AspectRatio - 0.1);

	std::cout << AspectRatio << std::endl;
	//std::cout <<"size.x = " << size.x << std::endl;

	//font size
	ImFontAtlas* atlas = ImGui::GetIO().Fonts;
	ImFont* font = atlas->Fonts[0];
	font->Scale = 1.0f * AspectRatio;



	ImGui::Image(my_background, ImVec2(TextureHandler::GetTextureData("Background_4")->width/2 * AspectRatio, TextureHandler::GetTextureData("Background_4")->height/2 * AspectRatio), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));

	widgetPos = ImVec2(size.x * 2/5 - my_file_w/2, size.y * 1/2 - my_file_h/2);
	ImGui::SetCursorPos(widgetPos);
	ImGui::Image(my_LeakyLogo, ImVec2(my_file_w , my_file_h), ImVec2(0.25, 0.25), ImVec2(0.75, 0.75), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));

	widgetPos = ImVec2(size.x * 0.0f, size.y * 91/100 - my_EngineLogo_h/2 );//I know, weird fraction is just te be consistent
	ImGui::SetCursorPos(widgetPos);
	ImGui::Image(my_EngineLogo,ImVec2(my_EngineLogo_w, my_EngineLogo_h),ImVec2(0.2,0.2),ImVec2(0.8,0.8), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));

	widgetPos = ImVec2(size.x* 88/100, size.y * 93/100);
	ImGui::SetCursorPos(widgetPos);

	ImGui::Checkbox("Show Cursor ", &showMouse);


	
	
	if (Input::GetInstance()->WasKeyReleased(80 | 112)) {

		if (showMouse) {
			w->MouseShowCursor = false;
			w->ShowCursor();
			showMouse = false;
		}
		else {

			w->MouseShowCursor = true;
			w->ShowCursor();
			showMouse = true;
		}

	}
	
	

	if (ImGui::BeginMenuBar()) {
		

		if (ImGui::BeginMenu("Window")) {

			MainMenuOptions();
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Settings"))
		{
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//ImGui::Text("Basic Main menu");              

	widgetPos = ImVec2(size.x * 7/10, size.y * 1/4 - buttonSize.y/2);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("   Play", buttonSize))
		gameRef->SetState(GameState::In_Game);

	widgetPos = ImVec2(size.x * 7 / 10, size.y * 1/2 - buttonSize.y/2);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("Options", buttonSize))                            
		gameRef->SetState(GameState::Options_Menu);

	widgetPos = ImVec2(size.x * 7 / 10, size.y * 3/4 - buttonSize.y/2);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("Exit", buttonSize)) {                          
		SDL_Quit();
		V3Engine::GetInstance()->Shutdown();
	}

	//width and height of Anim
	float w = TextureHandler::GetTextureData("DancingAnim")->width;
	float h = TextureHandler::GetTextureData("DancingAnim")->height;

	//Animations
	static std::array<ImVec2, 8> AnimFrame = { ImVec2(0,1),ImVec2(1,2),ImVec2(2,3),ImVec2(3,4),ImVec2(4,5),ImVec2(5,6),ImVec2(6,7),ImVec2(7,8) };
	static std::array<ImVec2, 8> AnimFrame_Kick = { ImVec2(0,1),ImVec2(1,2),ImVec2(2,3),ImVec2(3,4),ImVec2(4,5) };



	static int dancing_f = 0;
	static int kick_f = 0;


	static float OldTime = ImGui::GetTime();
	


	//Reset Anim Frame
	if (dancing_f == 7) { dancing_f = 1; }
	//Set Position of widget
	widgetPos = ImVec2(size.x * 7 / 10 , size.y * 1 / 4 - buttonSize.y / 2);
	ImGui::SetCursorPos(widgetPos);
	ImGui::Image(Anim_Dancing, ImVec2((190.0f / 4) * AspectRatio, (170.0f / 4) * AspectRatio), ImVec2((190.0f / w * AnimFrame[dancing_f].x), 0.0), ImVec2((190.0f / w) * AnimFrame[dancing_f].y, 170.0 / h), ImColor(255, 255, 255, 255), ImColor(1, 1, 1, 1)); ImGui::SameLine();
	//update net size of sprites
	w = TextureHandler::GetTextureData("KickAnim")->width;
	h = TextureHandler::GetTextureData("KickAnim")->height;

	if (kick_f == 4) { kick_f = 1; }
	widgetPos = ImVec2(size.x * 7 / 10, size.y * 3 / 4 - buttonSize.y / 2);
	ImGui::SetCursorPos(widgetPos);
	ImGui::Image(Anim_kick, ImVec2((190.0f/4) * AspectRatio, (180.0f/4) * AspectRatio), ImVec2((190.0f / w * AnimFrame[kick_f].x), 0.0), ImVec2((190.0f / w) * AnimFrame[kick_f].y, 180.0 / h), ImColor(255, 255, 255, 255), ImColor(1, 1, 1, 1));


	w = TextureHandler::GetTextureData("WiggleAnim")->width;
	h = TextureHandler::GetTextureData("WiggleAnim")->height;

	widgetPos = ImVec2(size.x * 7 / 10, size.y * 1 / 2 - buttonSize.y / 2);
	ImGui::SetCursorPos(widgetPos);
	ImGui::Image(Anim_Wiggle, ImVec2(160.0f/4 * AspectRatio, 170.0f/4 * AspectRatio), ImVec2((160.0f / w * AnimFrame[dancing_f].x), 0.0), ImVec2((160.0f / w) * AnimFrame[dancing_f].y, 170.0 / h), ImColor(255, 255, 255, 255), ImColor(1, 1, 1, 1));



	if (OldTime <= ImGui::GetTime()) {

		dancing_f++;
		kick_f++;
		OldTime = ImGui::GetTime() + 0.135;

	}


	widgetPos = ImVec2(0,0);
	ImGui::SetCursorPos(widgetPos);
	ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);





}

void MainMenuUI::Update(const float deltaTime_)
{
	imGuiScene("MainMenu");
}

void MainMenuUI::MainMenuOptions()
{
	ImGui::MenuItem("(test menu)", NULL, false, false);
	ImGui::Separator();//just a line for organization purposes
	if (ImGui::BeginMenu("Options")) {
	
		ImGui::Checkbox("Rezise", &no_resize);
		ImGui::Checkbox("Collapse", &no_collapse);
		ImGui::Checkbox("Auto_Resize", &auto_resize);
		ImGui::Checkbox("ScrollBar", &no_scrollbar);
		ImGui::EndMenu();
		
	}


}

void MainMenuUI::WindowStyle(ImGuiStyle* ref) {
	//get the current style
	ImGuiStyle& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;


	/**WINDOW STYLE SETTINGS**/
		//Sizes

		//ButtonsRounding
		style.FrameRounding = 12.f;
		//sliders cuadrado o redondo
		style.GrabRounding = 0.0f; 
		//win padding
		style.WindowPadding.x = 0.0f;
		style.WindowPadding.y = 0.0f;

		//Colors
		style.Colors[0] = ImVec4(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1);//Font color
		style.Colors[2] = ImVec4(0.0, 0.0, 0.0, 102.0/255.0);//Background
		//sliders etc
		style.Colors[7] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 99.0 / 255.0);//Frame Bg
		style.Colors[8] = ImVec4(215.0 / 255.0, 103.0 / 255.0, 0.0, 102.0 / 255.0);//Frame Bg Hovered
		style.Colors[9] = ImVec4(215.0 / 255.0, 103.0 / 255.0, 0.0, 102.0 / 255.0);//frame bg active
		//pop up windows NOTE: all the lists are pop up windows
		style.Colors[5] = ImVec4(228.0 / 255.0, 96.0 / 255.0, 9.0 / 255.0, 102.0/255.0);//pop up
		//title
		style.Colors[11] = ImVec4(0.0, 0.0, 0.0, 1);//title bg
		style.Colors[12] = ImVec4(0.0, 0.0, 0.0, 1);//title bg collapsed

		style.Colors[13] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 255.0 / 255.0);//Background menu bar
		style.Colors[21] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 99.0/255.0);//Background
		style.Colors[22] = ImVec4(0.0/255.0, 153.0 / 255.0, 153.0/255.0, 202.0 / 255.0);//hover buttons
		style.Colors[23] = ImVec4(218.0/255.0, 136.0 / 255.0,30.0/255.0, 200.0/255.0);//Background


		//style
		style.WindowRounding = 0.0f;




}

float MainMenuUI::CalculateDisplayChange(float initial_, float final_) {
	float change;
	
	return change = initial_ / final_;

}
