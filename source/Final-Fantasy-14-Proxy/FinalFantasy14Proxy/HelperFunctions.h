#pragma once
#include "pch.h"
#include <Psapi.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include <d3d11.h>
#include <dxgi.h>


namespace HelperFunctions
{
	MODULEINFO GetModuleInfo(const wchar_t* szModule);
	DWORD64 FindPattern(const wchar_t* module, const char* pattern, const char* mask);
	HWND find_main_window();
};

char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

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

struct vec3ff
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
};