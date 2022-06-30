#pragma once

#define MYMENU_EXIT (WM_APP + 100)


/*
I'm not sure why, but for some reason ImGui doesn't like it when I use pre-compiled headers...
I therefore made a hmenu as a backup. It was easy to make an needs only winapi...
*/
namespace Hmenu
{
	HMENU CreateDLLWindowMenu();
	BOOL RegisterDLLWindowClass(const wchar_t szClassName[], HMODULE hModule);
	void create_window(HMODULE hModule);
}