#include "OptionsMenuUI.h"
#include <array>
static float r = 0.0f;
static float g = 0.0f;
static float b = 0.0f;
static float f = 0.0f;

//image test
static float up = 0.0f;
static float left = 0.0f;
static float down = 0.0f;
static float right = 0.0f;

OptionsMenuUI::OptionsMenuUI()
{
}


OptionsMenuUI::~OptionsMenuUI()
{
	//clean up 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

}





void OptionsMenuUI::imGuiScene(const char * frame_name_)
{

		//Flags for the window
		static bool no_resize = true;
		static bool no_move = true;//this might cause issues when the window change w and h 
		static bool no_collapse = true;
		static bool auto_resize = true;
		static bool no_menu = true;
		static bool no_titlebar = true;
		static bool no_scrollbarr = false;
		//options menu 
		static bool Windowed = true;
		static bool FullScreen = false;
		static int ControllerSensitivity = 5.0;

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

		ImGui::SliderFloat("", &ue_Slider, 0.0f, 1024.0f);
		ImGui::SliderFloat("##2", &ue_slider, 0.0f, 576.0f);
		ImGui::Checkbox("mybool", &ue_Bool);
		ImGui::Checkbox("mybool_1", &ue_Bool_2);
		ImGui::Checkbox("mybool_2", &ue_Bool_3);
		ImGui::Checkbox("mybool_3", &ue_Bool_4);

		//end of testing UI
		//resolutions		
		static ImVec2 Resolutions[] = { ImVec2(1024, 576) , ImVec2(1152,648),ImVec2(1280,720),ImVec2(1920,1080)};//chage this for a vector and take all the resolutions

		//setting the size and position of our window
		ImVec2 size = ImVec2(w->GetCurrentResolution().w, w->GetCurrentResolution().h);//window size -> later update this with the width and height from the window

		ImVec2 defaultSize = ImVec2(1024, 576);
		

		if (ActualResolution == nullptr) {
			ActualResolution = &defaultSize;
		}
		size = *ActualResolution;
		Resolutions[0] = ImVec2(1024, 576);


		Resolutions[1].x = w->GetWindowResolutions()[16].w;
		Resolutions[1].y = w->GetWindowResolutions()[16].h;

		Resolutions[2].x = w->GetWindowResolutions()[8].w;
		Resolutions[2].y = w->GetWindowResolutions()[8].h;

		Resolutions[3].x = w->GetWindowResolutions()[0].w;
		Resolutions[3].y = w->GetWindowResolutions()[0].h;
		
			float AspectRatio = CalculateDisplayChange(defaultSize.x, size.x);

			ImVec2 buttonSize = ImVec2(150.0f * AspectRatio, 50 * AspectRatio);
			ImVec2 pos = ImVec2(0, 0);
			ImVec2 widgetPos = ImVec2(0, 0);




			//size and position of the window 
			ImGui::SetNextWindowSize(size);
			ImGui::SetNextWindowPos(pos);


			//activate windows flags
			ImGuiWindowFlags window_flags = 0;

			//flags go here
			if (no_scrollbarr)       window_flags |= ImGuiWindowFlags_NoScrollbar;
			//if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
			//if (no_resize)			window_flags |= ImGuiWindowFlags_NoResize;
			if (auto_resize)		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
			if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
			if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
			if (no_titlebar)		window_flags |= ImGuiWindowFlags_NoTitleBar;

			ImGui::Begin("Options Menu", false, window_flags);
			WindowStyle(&ImGui::GetStyle());
			//Textures
			ImTextureID my_background = (void*)(size_t)TextureHandler::GetTexture("Background_1");
			ImTextureID Anim_Water = (void*)(size_t)TextureHandler::GetTexture("WaterAnim");
			ImTextureID Anim_kick = (void*)(size_t)TextureHandler::GetTexture("KickAnim");
			ImTextureID Anim_Wiggle = (void*)(size_t)TextureHandler::GetTexture("WiggleAnim");

			//font size
			ImFontAtlas* atlas = ImGui::GetIO().Fonts;
			ImFont* font = atlas->Fonts[0];
			font->Scale = 1.0f * AspectRatio;



			ImGui::Image(my_background, ImVec2(TextureHandler::GetTextureData("Background_1")->width / 2 * AspectRatio, TextureHandler::GetTextureData("Background_1")->height / 3 * AspectRatio), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));



			//widgetPos = ImVec2(size.x * 7 / 10, size.y * 1 / 2);
			
			widgetPos = ImVec2(size.x/3.3, size.y / 5.5);
			ImGui::SetCursorPos(widgetPos);
			const char* items[] = { "1024X576", "1152X648", "1280X720", "1920X1080" };

			static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("Resolution", item_current)) // The second parameter is the label previewed before opening the combo.
			{
				ImGui::SameLine(); ImGui::Text("^ Press the arrow again to apply changes");
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (item_current == items[n]);
					if (ImGui::Selectable(items[n], is_selected)) {
						item_current = items[n];

					}
					if (is_selected) {
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
						switch (n)
						{

						case 0:
							ActualResolution = &Resolutions[0];
							w->SetWindowResolution(0);
							break;
						case 1:
							ActualResolution = &Resolutions[1];
							
							break;
						case 2:
							ActualResolution = &Resolutions[2];
							
							break;
						case 3:
							ActualResolution = &Resolutions[3];
							
							break;
						default:
							//ActualResolution = &Resolutions[0];
							break;
						}
						
					}
					
				}
				ImGui::EndCombo();
			}

			widgetPos = ImVec2(size.x /3.3, size.y/4);
			ImGui::SetCursorPos(widgetPos);

			ImGui::Checkbox("Windowed", &Windowed);

			//widgetPos = ImVec2(size.x / 1.3, size.y / 1.7);
			//ImGui::SetWindowPos(widgetPos);

			ImGui::SameLine(); ImGui::Checkbox("FullScreen", &FullScreen);
			widgetPos = ImVec2(size.x / 3.3, size.y / 3);
			ImGui::SetCursorPos(widgetPos);
			ImGui::SliderInt("Controller Sensitivity", &ControllerSensitivity, 1, 10, "%d");


			widgetPos = ImVec2(size.x/1.3, size.y/1.27 );
			ImGui::SetCursorPos(widgetPos);
			if (ImGui::Button("Back", buttonSize))
				UIstate = GameState::Main_Menu;

			static std::array<ImVec2, 16> AnimFrame = { ImVec2(0,1),ImVec2(1,2),ImVec2(2,3),ImVec2(3,4),ImVec2(4,5),ImVec2(5,6),ImVec2(6,7),ImVec2(7,8),ImVec2(8,9),ImVec2(9,10),ImVec2(10,11),ImVec2(11,12),ImVec2(12,13),ImVec2(13,14),ImVec2(14,15),ImVec2(15,16) };
			static std::array<ImVec2, 8> AnimFrame_wiggle = { ImVec2(0,1),ImVec2(1,2),ImVec2(2,3),ImVec2(3,4),ImVec2(4,5),ImVec2(5,6),ImVec2(6,7),ImVec2(7,8) };
			static std::array<ImVec2, 8> AnimFrame_kick = { ImVec2(0,1),ImVec2(1,2),ImVec2(2,3),ImVec2(3,4),ImVec2(4,5) };

			float w = TextureHandler::GetTextureData("WaterAnim")->width;
			float h = TextureHandler::GetTextureData("WaterAnim")->height;



			static int water_f = 0;
			static int wiggle_f = 0;

			static float OldTime = ImGui::GetTime();

			

			widgetPos = ImVec2(size.x / 1.15, size.y /1.15);
			ImGui::SetCursorPos(widgetPos);
			if (water_f == 15) { water_f = 1; }
			ImGui::Image(Anim_Water, ImVec2(190.0f/4, 200.0f/4), ImVec2((190.0f / w * AnimFrame[water_f].x), 0.0), ImVec2((190.0f / w) * AnimFrame[water_f].y, 200.0 / h), ImColor(255, 255, 255, 255), ImColor(1, 1, 1, 1)); 


			
			w = TextureHandler::GetTextureData("WiggleAnim")->width;
			h = TextureHandler::GetTextureData("WiggleAnim")->height;
			widgetPos = ImVec2(size.x / 3.9, size.y / 5.6);
			ImGui::SetCursorPos(widgetPos);
			if (wiggle_f == 7) { wiggle_f = 1; }
			ImGui::Image(Anim_Wiggle, ImVec2(160.0f/4, 170.0f/4), ImVec2((160.0f / w * AnimFrame_wiggle[wiggle_f].x), 0.0), ImVec2((160.0f / w) * AnimFrame_wiggle[wiggle_f].y, 170.0 / h), ImColor(255, 255, 255, 255), ImColor(1, 1, 1, 1));


			if (OldTime <= ImGui::GetTime()) {

				water_f++;
				wiggle_f++;
				OldTime = ImGui::GetTime() + 0.135;
			
			}

}

void OptionsMenuUI::Update(const float deltaTime_)
{
	imGuiScene("OptionsMenu");
}

void OptionsMenuUI::WindowStyle(ImGuiStyle* ref) {
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
	style.Colors[2] = ImVec4(0.0, 0.0, 0.0, 102.0 / 255.0);//Background
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

float OptionsMenuUI::CalculateDisplayChange(float initial_, float final_) {
	float change;

	return change = initial_ / final_;

}

