#pragma once
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
#include <d3d11.h>
#include <Windows.h>
class InputHook
{
public:
	void Init(HWND hWindow);
	void Remove(HWND hWindow);

};

static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);