#include "OptionsMenu.h"

static float r = 0.0f;
static float g = 0.0f;
static float b = 0.0f;
static float f = 0.0f;

//image test
static float up = 0.0f;
static float left = 0.0f;
static float down = 0.0f;
static float right = 0.0f;

OptionsMenu::OptionsMenu()
{
}


OptionsMenu::~OptionsMenu()
{
	//clean up 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

}



static bool no_scrollbarr = false;

void OptionsMenu::imGuiScene(const char * frame_name_)
{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		//setting the size and position of our window
		ImVec2 size = ImVec2(800, 600);//window size -> later update this with the width and height from the window
		ImVec2 buttonSize = ImVec2(200, 50);
		ImVec2 pos = ImVec2(0, 0);
		ImGui::SetNextWindowSize(size);
		ImGui::SetNextWindowPos(pos);
		ImVec2 widgetPos = ImVec2(0, 0);
		//activate windows flags
		ImGuiWindowFlags window_flags = 0;

		//flags go here
		if (no_scrollbarr)       window_flags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("Main Menu",false, window_flags);       

		if (ImGui::TreeNode("Basic Options Menu")) {
			ImGui::Text("Color Test");

			//color
			widgetPos = ImVec2((size.x / 2) - 300, 100);
			ImGui::SetCursorPos(widgetPos);
			ImGui::SliderFloat("R", &r, 0.0f, 1.0f);
			widgetPos = ImVec2((size.x / 2) - 300, 150);
			ImGui::SetCursorPos(widgetPos);
			ImGui::SliderFloat("G", &g, 0.0f, 1.0f);
			widgetPos = ImVec2((size.x / 2) - 300, 200);
			ImGui::SetCursorPos(widgetPos);
			ImGui::SliderFloat("B", &b, 0.0f, 1.0f);


			widgetPos = ImVec2((size.x) - 150, 105);
			ImGui::SetCursorPos(widgetPos);
			ImGui::ColorButton("Interface color", ImVec4(r, g, b, 0.0), 0, ImVec2(110, 110));

			widgetPos = ImVec2(size.x / 2 - (buttonSize.x / 2), 300);
			ImGui::SetCursorPos(widgetPos);

			if (ImGui::Button("Back", buttonSize))
				UIstate = GameState::Main_Menu;

			ImGui::TreePop();
		}
		//images test
		/*if (ImGui::TreeNode("Images"))
		{
			ImGuiIO& io = ImGui::GetIO();

			ImTextureID my_tex_id = io.Fonts->TexID;
			ImTextureID my_file = (void*)(size_t)TextureHandler::GetTexture("LeakyLogo");
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			float buttonH = 100;
			float widthW = 100;

			/*ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::Image(my_file, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));*/
			/*ImGui::PushItemWidth(200);
			ImGui::SliderFloat("Scale Left", &left, 0.0f, 1.0f);
			ImGui::SliderFloat("Scale Up", &up, 0.0f, 1.0f);
			ImGui::SliderFloat("Scale Right", &right, 0.0f, 1.0f);
			ImGui::SliderFloat("Scale Down", &down, 0.0f, 1.0f);
		
			
			ImGui::ImageButton(my_file, ImVec2(widthW, buttonH), ImVec2(left, up), ImVec2(right, down), 5, ImColor(r, g, b, f), ImVec4(1, 1, 1, 1));

			ImGui::TextWrapped("And now some textured buttons..");
			static int pressed_count = 0;
			for (int i = 0; i < 8; i++)
			{
				ImGui::PushID(i);
				int frame_padding = -1 + i;     // -1 = uses default padding
				if (ImGui::ImageButton(my_file, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f / my_tex_w, 32 / my_tex_h), frame_padding, ImColor(0, 0, 0, 255)))
					pressed_count += 1;
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
			ImGui::Text("Pressed %d times.", pressed_count);
			ImGui::TreePop();
		}

		*/

}

void OptionsMenu::Update(const float deltaTime_)
{
	imGuiScene("OptionsMenu");
}
