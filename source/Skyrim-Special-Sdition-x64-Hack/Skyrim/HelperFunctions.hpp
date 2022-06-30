#pragma once

#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <vector>
#include <string>
#include <imgui.h>

#define PROCESS L"SkyrimSE.exe"

#define VK_W 0x57
#define	VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44


constexpr float mapCoordScale = 70.0f;

struct vec4
{
	float x, y, z, w;
};
struct vec3
{
	float x, y, z;
};
struct vec2
{
	float x, y;
};

namespace HelperFunctions
{
	MODULEINFO GetModuleInfo(const wchar_t* szModule);
	DWORD64 PatternScan(const wchar_t* module, const char* signature);
	HWND find_main_window();
	bool worldToScreen(vec3 pos, vec2& screen, float matrix[16], int m_windowWidth, int m_windowHeight);
};

struct CheatVars
{
	ImVec4 espColors = { 1, 0, 0 , 1 };
	float flySpeed = 1.0f;
	float runSpeed = 1.0f;
};
inline CheatVars cheatVars;