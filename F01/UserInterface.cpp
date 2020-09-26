#include "UserInterface.h"


using namespace ImGui;


bool UserInterface::IsOpen = true;

bool UserInterface::general_cooldown;
bool UserInterface::general_reach;
bool UserInterface::general_enableFOV;
float UserInterface::general_fov = 1.0;
bool UserInterface::general_teamdamage;
int UserInterface::general_teamnumber;

bool UserInterface::movement_enable;
float UserInterface::movement_walkspeed = 1;
float UserInterface::movement_mass = 100;
float UserInterface::movement_aircontrol = 0.25f;
bool UserInterface::movement_gravity = 1;
bool UserInterface::movement_fly;
bool UserInterface::movement_jumpcount;

int UserInterface::character_select;
bool UserInterface::character_change;

bool UserInterface::enabled;
bool UserInterface::character_select_enabled;

void UserInterface::ShowGeneralMenu()
{
	
	//static ImVec4 color = ImVec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);
	ImGui::Checkbox("FOV Modifier", &general_enableFOV); ImGui::SameLine();
	ImGui::SliderFloat("", &general_fov, -1, 500);
	ImGui::Checkbox("No Cooldown", &general_cooldown);
	ImGui::Checkbox("Super Reach", &general_reach);
	ImGui::Checkbox("Team Damage", &general_teamdamage);
	ImGui::SliderInt("Team Number", &general_teamnumber, 0, 7);
	
	ImGui::Separator();
}

void UserInterface::ShowMovementMenu()
{
	ImGui::Checkbox("Enable Modifiers", &movement_enable);
	ImGui::SliderFloat("Walk Speed", &movement_walkspeed, 0, 20);
	ImGui::SliderFloat("Mass", &movement_mass, 0, 1000);
	ImGui::SliderFloat("Air Control", &movement_aircontrol, 0, 5);
	ImGui::Checkbox("Gravity", &movement_gravity);
	ImGui::Checkbox("Fly", &movement_fly);
	ImGui::Checkbox("Air Jumping", &movement_jumpcount);

}

void UserInterface::ShowCharacterMenu()
{
	
	ImGui::Text("Can only be used in character selection screen");
	ImGui::Text("Enter Character ID");
	ImGui::InputInt("", &character_select); ImGui::SameLine(); 
	character_change = (ImGui::Button("Change") ? true : false);

	ImGui::Separator();

}
void UserInterface::ShowMiscMenu()
{
	ImGui::Text("Nothing here yet");
	ImGui::Separator();
}

void UserInterface::Render()
{
	static bool no_titlebar = false;
	static bool no_border = true;
	static bool no_resize = false;
	static bool auto_resize = false;
	static bool no_move = false;
	static bool no_scrollbar = false;
	static bool no_collapse = false;
	static bool no_menu = true;
	static bool start_pos_set = false;
	static bool no_close = false;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.61f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);




	//if (show_esp_menu) ShowESPMenu(&show_esp_menu);

	//ImGui::GetIO().MouseDrawCursor = IsOpen;

	ImGuiWindowFlags	window_flags = 0;
	if (no_titlebar)	window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_resize)		window_flags |= ImGuiWindowFlags_NoResize;
	if (auto_resize)	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (no_move)		window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar)	window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)	window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)		window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_close)		IsOpen = NULL;


	ImGuiIO& io = ImGui::GetIO(); 

	if (IsOpen) {

		
		io.WantCaptureMouse = true;

		ImGui::Begin("Aburasoft", &IsOpen, window_flags);
		
		ImGui::Checkbox("Enabled", &enabled);
		character_select_enabled = (ImGui::Button("MP Character Switching") ? true : false);

		if (ImGui::CollapsingHeader("General Menu"))
		{
			GetInstance().ShowGeneralMenu();
		}
		if (ImGui::CollapsingHeader("Movement Menu"))
		{
			GetInstance().ShowMovementMenu();
		}
		if (ImGui::CollapsingHeader("Character Menu"))
		{
			GetInstance().ShowCharacterMenu();
		}
		if (ImGui::CollapsingHeader("Misc"))
		{
			GetInstance().ShowMiscMenu();
		}

		ImGui::End();
	}
	else {
		io.WantCaptureMouse = false;
	}

   
	
	
}

UserInterface &UserInterface::GetInstance()
{
	static UserInterface instance;

	return instance;
}
