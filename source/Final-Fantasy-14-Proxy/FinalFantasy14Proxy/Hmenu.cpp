#include "pch.h"
#include "Hmenu.h"

LRESULT CALLBACK MessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    switch (uMessage)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case MYMENU_EXIT:
            PostQuitMessage(0);
            return 0;
            break;
        }
    }
    return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

namespace Hmenu
{
	HMENU CreateDLLWindowMenu() {
		HMENU hMenu = CreateMenu();
		HMENU hMenuPopup = CreatePopupMenu();
		if (hMenu == NULL)
			return FALSE;
		AppendMenuW(hMenuPopup, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("File"));
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("Edit"));
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("Help"));
		return hMenu;
	}

    BOOL RegisterDLLWindowClass(const wchar_t szClassName[], HMODULE hModule) {
        WNDCLASSEX wc;
        wc.hInstance = hModule;
        wc.lpszClassName = (LPCWSTR)szClassName;
        wc.lpfnWndProc = MessageHandler;
        wc.style = CS_VREDRAW | CS_HREDRAW;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
        wc.hIconSm = LoadIcon(NULL, IDI_SHIELD);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszMenuName = NULL;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
        if (!RegisterClassEx(&wc))
            return 0;
        return 1;
    }

    void create_window(HMODULE hModule)
    {
        HMENU hMenu = Hmenu::CreateDLLWindowMenu();
        Hmenu::RegisterDLLWindowClass(L"Proxy", hModule);
        HWND hwnd = CreateWindowEx(0, L"Proxy", L"Proxy", WS_EX_LAYERED, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, NULL, hMenu, hModule, NULL);
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }
}