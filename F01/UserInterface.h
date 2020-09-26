#pragma once
#include "Hooks.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <cstdint>

class UserInterface
{
public:

	UserInterface(UserInterface& const) = delete;
	void operator=(UserInterface const&) = delete;

	static bool IsOpen;
	static bool general_cooldown;
	static float general_fov;
	static bool general_reach;
	static bool general_enableFOV;
	static bool enabled;
	static bool general_teamdamage;
	static int general_teamnumber;

	static bool movement_enable;
	static float movement_walkspeed;
	static float movement_mass;
	static float movement_aircontrol;
	static bool movement_gravity;
	static bool movement_fly;
	static bool movement_jumpcount;

	static bool character_select_enabled;
	static int character_select;
	static bool character_change;

	static UserInterface& GetInstance();
	static void Render();

private:

	UserInterface() {}
	void ShowGeneralMenu();
	void ShowMovementMenu();
	void ShowCharacterMenu();
	void ShowMiscMenu();

};
