// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "HelperFunctions.h"
#include "Hook.h"
#include "Menu.h"
//#include "Hmenu.h"

extern bool menu_show_e;


DWORD64 WINAPI MainThread(HMODULE hModule)
{
	{
		Menu::start();
		//Hmenu::create_window(hModule);

		while (true)
		{
			if (GetAsyncKeyState(VK_DELETE) & 1)
			{
				std::cout << "[+] Unhooking and Unloading!" << std::endl;
				break;
			}
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				menu_show_e = !menu_show_e;
			}
			Sleep(1);
		}

		Menu::stop();
	}
	Sleep(500);
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

/*
    I'm using the Intel C++ Compiler 19.2 (ICC). I'm doing this because this gives me the option to use __asm{} in x64 aswell.
*/

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}