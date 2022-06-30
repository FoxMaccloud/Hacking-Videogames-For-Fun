#include "Menu.h"

RECT windowSize;
int window_width;
int window_height;

int imgui_window_width;
int imgui_window_height;
bool menu_show_e = false;
bool lock_inputs_e = false;

Proxy proxy;
Console console;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void get_window_size()
{
	auto game_window = GetWindowRect(HelperFunctions::find_main_window(), &windowSize);
	window_width = (int)(windowSize.right - windowSize.left);
	window_height = (int)(windowSize.bottom - windowSize.top);
	imgui_window_width = window_width - 16;
	imgui_window_height = window_height / 3;
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	Menu::Theme();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	// TODO: Return TRUE after imgui when mouse is hvoering over window or if textfield is selected.

	if (menu_show_e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (lock_inputs_e)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			return TRUE;
		}
		else
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			if (io.WantCaptureMouse || io.WantCaptureKeyboard)
				return TRUE;
		}
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{	
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			get_window_size();
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	
	get_window_size();

	if (menu_show_e)
	{
		//ImGui::GetIO().MouseDrawCursor = 1;


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//ImGui::BeginMainMenuBar();
		//{
		//	if (ImGui::BeginMenu("File"))
		//	{
		//		ImGui::MenuItem("(demo menu)", NULL, false, false);
		//		if (ImGui::MenuItem("New")) {}
		//		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		//		if (ImGui::MenuItem("Save As..")) {}
		//		ImGui::Separator();
		//		ImGui::Text("Version 0.9.1");
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::BeginMenu("Edit"))
		//	{
		//		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		//		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		//		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
		//		ImGui::EndMenu();
		//	}
		//	ImGui::EndMainMenuBar();
		//}

		ImGui::Begin(WINDOW_NAME, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::SetWindowPos(ImVec2(-1, 0), NULL);
		ImGui::SetWindowSize(ImVec2(imgui_window_width+2, imgui_window_height), NULL);

		//ImGui::Text(("V 0.0.1"));

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("(demo menu)", NULL, false, false);
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}
				ImGui::Separator();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		console.Draw();


		//ImGui::BeginTabBar(("FF14#TabBar"), ImGuiTabBarFlags_NoTabListScrollingButtons);
		//if (ImGui::BeginTabItem(("Misc"))) {

		//	ImGui::Text(("Miscellaneous Stuff!"));

		//	ImGui::EndTabItem();
		//}

		//if (ImGui::BeginTabItem(("Debug"))) {

		//	ImGui::Text("Debuging!");
		//	ImGui::EndTabItem();
		//}
		//ImGui::EndTabBar();
		//ImGui::End();


		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	else
	{
		//ImGui::GetIO().MouseDrawCursor = 0;
	}

	return oPresent(pSwapChain, SyncInterval, Flags);
}

namespace Menu {
	void start() {
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
		}
	}

	void stop() {
		menu_show_e = false;
		proxy.~Proxy();
		//kiero::unbind(8);
		kiero::shutdown();
	}

	void Theme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();

		style->ChildBorderSize = 1;
		style->WindowMinSize = ImVec2(600, 400);
		//style->WindowMinSize = ImVec2(imgui_window_width, imgui_window_height);
		style->WindowTitleAlign = ImVec2(0.5, 0.5);
		style->AntiAliasedFill = true;

		style->WindowRounding = 0;
		style->ChildRounding = 0;
		style->FrameRounding = 0;
		style->ScrollbarRounding = 0;
		style->TabRounding = 0;
		style->GrabRounding = 0;
		style->ScrollbarSize = 0.9;

		style->Colors[ImGuiCol_TitleBg] = ImColor(25, 25, 25, 230);
		style->Colors[ImGuiCol_TitleBgActive] = ImColor(25, 25, 25, 230);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(25, 25, 25, 130);

		style->Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 230);
		style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
		style->Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 255);

		style->Colors[ImGuiCol_FrameBg] = ImColor(25, 25, 25, 230);
		style->Colors[ImGuiCol_FrameBgActive] = ImColor(25, 25, 25, 230);
		style->Colors[ImGuiCol_FrameBgHovered] = ImColor(25, 25, 25, 230);

		style->Colors[ImGuiCol_Header] = ImColor(35, 35, 35, 230);
		style->Colors[ImGuiCol_HeaderActive] = ImColor(35, 35, 35, 230);
		style->Colors[ImGuiCol_HeaderHovered] = ImColor(35, 35, 35, 230);

		style->Colors[ImGuiCol_ResizeGrip] = ImColor(35, 35, 35, 255);
		style->Colors[ImGuiCol_ResizeGripActive] = ImColor(35, 35, 35, 255);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(35, 35, 35, 255);

		style->Colors[ImGuiCol_Button] = ImColor(37, 37, 37, 255);
		style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 41, 41, 255);
		style->Colors[ImGuiCol_ButtonHovered] = ImColor(37, 37, 37, 255);

		style->Colors[ImGuiCol_Tab] = ImColor(136, 0, 20, 140);
		style->Colors[ImGuiCol_TabHovered] = ImColor(136, 0, 20, 160);
		style->Colors[ImGuiCol_TabActive] = ImColor(136, 0, 20, 220);

		style->Colors[ImGuiCol_MenuBarBg] = ImColor(100, 0, 20, 90);

		style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);
		style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
	}
}