#include "MainMenuUI.h"



MainMenuUI::MainMenuUI()
{
}


MainMenuUI::~MainMenuUI()
{
	//clean up 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

}

//Flags for the window
static bool no_resize = false;
static bool no_move = false;//this might cause issues when the window change w and h 
static bool no_collapse = false;
static bool auto_resize = false;
static bool no_menu = false;
static bool no_scrollbar = false;

void MainMenuUI::imGuiScene(const char * frame_name_)
{
	static float f = 0.0f;
	static int counter = 10; 

	//if (!firstTime) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		 

		ImGui::NewFrame();
	//}

		
		//font loading 
		
		//UI FOR EDITIING
		static float ue_slider = 0.0;
		static float ue_Slider = 0.0f;
		static bool ue_Bool = false;
		ImGui::SliderFloat("", & ue_Slider, 0.0f, 800.0f);
		ImGui::SliderFloat("##2", &ue_slider, 0.0f, 600.0f);
		ImGui::Checkbox("mybool", &ue_Bool);

	
	//setting the size and position of our window//1280
	ImVec2 size = ImVec2(800 + ue_Slider ,600  + ue_slider);//window size later update this with teh sidth and height of the window
	//calculate aspect ration in case display changes 
	ImVec2 AspectRatio = ImVec2(size.x/size.y,1);

	ImVec2 buttonSize = ImVec2(150.0f * AspectRatio.x, 50 * AspectRatio.y);
	ImVec2 pos = ImVec2(0, 0);
	ImVec2 widgetPos = ImVec2(0, 0);



	ImGuiWindowFlags window_flags = 0;

	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)			window_flags |= ImGuiWindowFlags_NoResize;
	//window_flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	if (auto_resize)		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;

	//size and position of the window
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(pos);
	
	//creates a window
	ImGui::Begin("Main Menu", false, window_flags); 
	WindowStyle(&ImGui::GetStyle());


	//all files needed
	//Engine::Image::ID();
	//ImTextureID my_file = (void*)(size_t)TextureHandler::GetTexture("LeakyLogo");
	//ImTextureID my_file_2 = (void*)(size_t)TextureHandler::GetTexture("CheckerboardTexture");
	//float my_tex_w = TextureHandler::GetTextureData("LeakyLogo")->width/2;
	//float my_tex_h = TextureHandler::GetTextureData("LeakyLogo")->height /2;
	//ImVec2 ImgAspectRatio = ImVec2(size.x/my_tex_w,size.y/my_tex_h);
	//
	//std::cout << ImgAspectRatio.x << std::endl;
	//if (ue_Bool == false) {my_file_2 = (void*)(size_t)TextureHandler::GetTexture("CheckerboardTexture");}
	//else{my_file_2 = (void*)(size_t)TextureHandler::GetTexture("LeakyLogo");}




	widgetPos = ImVec2(0.0f, 0.0f);
	ImGui::SetCursorPos(widgetPos);
	//keep in mind if teh image aspect ratio is 1:1 they need to change the same amount than x
	//the number with the "-x" value represents teh initial size from wich there is going to be a reference to scale up or down dependiong on the display  
	//ImGui::Image(my_file_2, ImVec2((my_tex_w - 175) * ImgAspectRatio.x, (my_tex_h - 175) * ImgAspectRatio.x), ImVec2(0.25, 0.25), ImVec2(0.75, 0.75), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));
	/*static int pressed_count = 0;
	for (int i = 0; i < 8; i++)
	{
		ImGui::PushID(i);
		int frame_padding = -1 + i;     // -1 = uses default padding
		if (ImGui::ImageButton(my_file, ImVec2(800, 600), ImVec2(0.1, 0.1), ImVec2(0.9,0.9), frame_padding, ImColor(0, 0, 0, 255)))
			
			pressed_count += 1;
		ImGui::PopID();
	
	}*/

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

	ImGui::Text("Basic Main menu");              

	widgetPos = ImVec2(size.x * 7/10, size.y * 4/25);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("Play", buttonSize))                           
		counter++;

	widgetPos = ImVec2(size.x * 7 / 10, size.y * 1/3);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("Level Editor", buttonSize))                            
		counter++;

	widgetPos = ImVec2(size.x * 7 / 10, size.y * 1/2);
	ImGui::SetCursorPos(widgetPos);
	if (ImGui::Button("Options", buttonSize)) {                          
		UIstate = GameState::Options_Menu;
	}


	widgetPos = ImVec2(size.x - 350,size.y - 20);
	ImGui::SetCursorPos(widgetPos);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);





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

		//Color Bg
		style.Colors[2] = ImVec4(228.0/255.0, 96.0/255.0, 9.0/255.0, 1);



}		
