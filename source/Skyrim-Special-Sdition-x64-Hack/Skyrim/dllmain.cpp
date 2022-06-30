// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include "Menu.hpp"

DWORD64 WINAPI MainThread(HMODULE hModule)
{
    {
        std::unique_ptr<Menu> menu = std::make_unique<Menu>();
        while (true)
        {
            menu->render();
            if (GetAsyncKeyState(VK_DELETE) & 1)
            {
                break;
            }
            if (GetAsyncKeyState(VK_INSERT) & 1)
            {
                menu->toggle_menu();
            }
            Sleep(1);
        }
    }
    Sleep(500);
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

