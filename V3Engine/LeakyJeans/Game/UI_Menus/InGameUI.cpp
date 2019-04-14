#include "InGameUI.h"

InGameUI::InGameUI()
{

}

InGameUI::InGameUI(Game * game)
{
	g = game;
	players.push_back(static_cast<Jeans*>(g->GetGameObjects()["Jeans"])); //WL = Water Level
	players.push_back(static_cast<Jeans*>(g->GetGameObjects()["Jeans2"])); //WL = Water Level
}

InGameUI::~InGameUI()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	gameRef = nullptr;


}




void InGameUI::imGuiScene(const char * frame_name_)
{
	//Flags for the window
	static bool no_resize = true;
	static bool no_move = true;
	static bool no_collapse = true;
	static bool auto_resize = true;
	static bool no_menu = true;
	static bool no_scrollbar = true;
	static bool no_titlebar = true;


	static bool showMouse = true;
	static float waterLv_1 = players[0]->GetWaterLevel();
	static float waterLv_2 = players[1]->GetWaterLevel();


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();


	ImVec2 size = ImVec2(w->GetCurrentResolution().w, w->GetCurrentResolution().h);//window size later update this with the sidth and height of the window

//calculate aspect ration in case display changes; 
//calculates the windows change so we can relate that number with all the elements exposed in the UI/// NOTE: I CAN CREATE AN ENUM WITH ALL THE RESOLUTIONS
	float AspectRatio = CalculateDisplayChange(800, size.x);


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
	ImGui::Begin("InGameMenu", false, window_flags);
	WindowStyle(&ImGui::GetStyle());


	//font size
	ImFontAtlas* atlas = ImGui::GetIO().Fonts;
	ImFont* font = atlas->Fonts[0];
	font->Scale = 1.0f * AspectRatio;




	widgetPos = ImVec2(size.x * 88 / 100, size.y * 93 / 100);
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

	widgetPos = ImVec2(0.0,0.0);
	ImGui::SetCursorPos(widgetPos);
	ImGui::SliderFloat("WaterLevel", &waterLv_1, 0.1, 100.0, "%.1");
	widgetPos = ImVec2(0.5, 0.5);
	ImGui::SliderFloat("WaterLevel", &waterLv_2, 0.1, 100.0, "%.1");









}

void InGameUI::Update(const float deltaTime_)
{
	imGuiScene("InGameMenu");
}

void InGameUI::MainMenuOptions()
{
}


void InGameUI::WindowStyle(ImGuiStyle * ref)
{

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
	style.Colors[2] = ImVec4(0.0, 0.0, 0.0, 0.0 / 255.0);//Background
	//sliders etc
	style.Colors[7] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 99.0 / 255.0);//Frame Bg
	style.Colors[8] = ImVec4(215.0 / 255.0, 103.0 / 255.0, 0.0, 102.0 / 255.0);//Frame Bg Hovered
	style.Colors[9] = ImVec4(215.0 / 255.0, 103.0 / 255.0, 0.0, 102.0 / 255.0);//frame bg active
	//pop up windows NOTE: all the lists are pop up windows
	style.Colors[5] = ImVec4(228.0 / 255.0, 96.0 / 255.0, 9.0 / 255.0, 102.0 / 255.0);//pop up
	//title
	style.Colors[11] = ImVec4(0.0, 0.0, 0.0, 1);//title bg
	style.Colors[12] = ImVec4(0.0, 0.0, 0.0, 1);//title bg collapsed

	style.Colors[13] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 255.0 / 255.0);//Background menu bar
	style.Colors[21] = ImVec4(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0, 99.0 / 255.0);//Background
	style.Colors[22] = ImVec4(0.0 / 255.0, 153.0 / 255.0, 153.0 / 255.0, 202.0 / 255.0);//hover buttons
	style.Colors[23] = ImVec4(218.0 / 255.0, 136.0 / 255.0, 30.0 / 255.0, 200.0 / 255.0);//Background


	//style
	style.WindowRounding = 0.0f;



}

float InGameUI::CalculateDisplayChange(float initial_, float final_)
{
	float change;

	return change = initial_ / final_;
}
