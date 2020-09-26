#pragma once
#include "UserInterface.h"
#include "InputHook.h"

#include <MinHook.h>
#include <D3D11.h>
#include <DXGI.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <Windows.h>


void CreateRenderTarget();
int Initialize_Hooks(HMODULE hModule, HWND hwnd);
int Shutdown_Hooks(HMODULE hModule, HWND hwnd);









