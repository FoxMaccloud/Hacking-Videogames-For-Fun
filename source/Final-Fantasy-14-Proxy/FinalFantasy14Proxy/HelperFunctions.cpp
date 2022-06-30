#include "pch.h"
#include "HelperFunctions.h"

struct handle_data
{
	unsigned long process_id;
	HWND window_handle;
};

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lparam) {
	auto& data = *reinterpret_cast<handle_data*>(lparam);
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);

	if (data.process_id != process_id)
	{
		return TRUE;
	}
	data.window_handle = handle;
	return FALSE;
}

namespace HelperFunctions {

	MODULEINFO GetModuleInfo(const wchar_t* szModule)
	{
		MODULEINFO modinfo = { 0 };
		HMODULE hModule = GetModuleHandle(szModule);
		if (hModule == 0)
			return modinfo;
		GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
		return modinfo;
	}

	// Signature based search.
	DWORD64 FindPattern(const wchar_t* module, const char* pattern, const char* mask)
	{
		MODULEINFO mInfo = GetModuleInfo(module);
		DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
		DWORD64 size = (DWORD64)mInfo.SizeOfImage;
		DWORD64 patternLength = (DWORD64)strlen(mask);
		for (DWORD64 i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (DWORD64 j = 0; j < patternLength; j++)
			{
				found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
			}
			if (found)
			{
				return base + i;
			}
		}
		return NULL;
	}

	// Get's a handle for the process which the dll is injected into.
	HWND find_main_window()
	{
		handle_data data{};

		data.process_id = GetCurrentProcessId();
		data.window_handle = nullptr;
		EnumWindows(enum_windows_callback, reinterpret_cast<LPARAM>(&data));

		return data.window_handle;
	}
}