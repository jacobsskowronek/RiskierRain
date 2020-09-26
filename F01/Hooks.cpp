#include "Hooks.h"
#include <iostream>
#include "Cheats.h"
const int UninjectLibraryKey = VK_DELETE;
const int OpenMenuKey = VK_F9;
const int DisableKey = VK_HOME;

InputHook inputHook = InputHook();

typedef HRESULT(WINAPI* D3D11PRESENT)(IDXGISwapChain*, UINT, UINT);


D3D11PRESENT oD3D11Present = NULL;

bool bCallOnce = false;


ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D11RenderTargetView* pRenderTargetView = NULL;

DXGI_SWAP_CHAIN_DESC sd;
bool show_demo_window = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

HWND hWindow;




HRESULT WINAPI DetourD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!bCallOnce) {

		pSwapChain->GetDesc(&sd);
		hWindow = sd.OutputWindow;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}
		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
		{
			pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &pRenderTargetView);
			renderTargetTexture->Release();
		}



		// ImGui stuff
		ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(pDevice, pContext);

		


		bCallOnce = true;
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Any application code here


	if (GetAsyncKeyState(OpenMenuKey) & 0x1) {
		UserInterface::GetInstance().IsOpen ? UserInterface::GetInstance().IsOpen = false : UserInterface::GetInstance().IsOpen = true;
		if (UserInterface::GetInstance().IsOpen)
			inputHook.Init(hWindow);
		else
			inputHook.Remove(hWindow);
		
	}

	if (GetAsyncKeyState(DisableKey) & 0x1) {
		UserInterface::enabled = false;
	}

	// Main cheat loop
	CheatLoop();

	// TODO: Get Instance of UI
	UserInterface::GetInstance().Render();
	//Draw::GetInstance()->BeginScene();
	//Draw::GetInstance()->DrawScene();
	//Draw::GetInstance()->EndScene();




	// Render dear imgui into screen
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	//pContext->ClearRenderTargetView(pRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//pSwapChain->Present(0, 0);


	return oD3D11Present(pSwapChain, SyncInterval, Flags);
}


int Initialize_Hooks(HMODULE hModule, HWND hwnd)
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	//ImGuiIO& io = ImGui::GetIO();


	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	//scd.Windowed = ((GetWindowLongPtr(hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;


	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		NULL, &featureLevel, 1, D3D11_SDK_VERSION,
		&scd, &pSwapChain,
		&pDevice, NULL, &pContext
	))) {
		//std::cout << "Failed to create device and swap chain" << std::endl;
		//MessageBoxA(NULL, "Failed to create device and swap chain", "D3D11CreateDeviceAndSwapChain", NULL);
		return 1;
	}

	


	DWORD_PTR* pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChain);
	DWORD_PTR* pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDevice);
	DWORD_PTR* pContextVT = reinterpret_cast<DWORD_PTR*>(pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);


	MH_Initialize();
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVT[8], DetourD3D11Present, reinterpret_cast<void**>(&oD3D11Present)) != MH_OK) {
		//std::cout << "Failed to create present hook" << std::endl;
		//MessageBoxA(NULL, "Failed to create present hook", "MH_CreateHook", NULL);
		return 1;
	}
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVT[8]) != MH_OK) {
		//std::cout << "Failed to enable present hook" << std::endl;
		//MessageBoxA(NULL, "Failed to enable present hook", "MH_EnableHook", NULL);
		return 1;
	}

	DWORD old_protect;
	VirtualProtect(oD3D11Present, 2, PAGE_EXECUTE_READWRITE, &old_protect);


	CreateRenderTarget();


	do {
		Sleep(100);
	} while (!(GetAsyncKeyState(UninjectLibraryKey) & 0x1));

	Shutdown_Hooks(hModule, hwnd);
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = NULL; }
}

int Shutdown_Hooks(HMODULE hModule, HWND hwnd)
{
	CleanupRenderTarget();
	
	if (UserInterface::GetInstance().IsOpen)
		inputHook.Remove(hWindow);
		
	
	if (pContext) pContext->Release();
	if (pSwapChain) pSwapChain->Release();
	if (pDevice) pDevice->Release();
	//inputHook.Remove(hwnd);
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();


	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}