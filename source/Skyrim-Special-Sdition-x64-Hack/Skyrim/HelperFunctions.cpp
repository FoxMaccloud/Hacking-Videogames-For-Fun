#include "HelperFunctions.hpp"
struct handle_data
{
	unsigned long process_id;
	HWND window_handle;
};

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lparam)
{
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

	// Get's a handle for the process which the dll is injected into.
	HWND find_main_window()
	{
		handle_data data{};

		data.process_id = GetCurrentProcessId();
		data.window_handle = nullptr;
		EnumWindows(enum_windows_callback, reinterpret_cast<LPARAM>(&data));

		return data.window_handle;
	}

	bool worldToScreen(vec3 pos, vec2& screen, float matrix[16], int m_windowWidth, int m_windowHeight)
	{
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		vec4 clipCoords;
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
		clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
		clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
		clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

		if (clipCoords.w < 120.0f)
			return false;

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		vec3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = (m_windowWidth / 2 * NDC.x) + (NDC.x + m_windowWidth / 2);
		screen.y = -(m_windowHeight / 2 * NDC.y) + (NDC.y + m_windowHeight / 2);
		return true;
	}

	DWORD64 PatternScan(const wchar_t* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<char>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back('\?');
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		MODULEINFO mInfo = GetModuleInfo(module);
		DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
		DWORD64 sizeOfImage = (DWORD64)mInfo.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);

		DWORD64 patternLength = patternBytes.size();
		auto data = patternBytes.data();

		for (DWORD64 i = 0; i < sizeOfImage - patternLength; i++)
		{
			bool found = true;
			for (DWORD64 j = 0; j < patternLength; j++)
			{
				char a = '\?';
				char b = *(char*)(base + i + j);
				found &= data[j] == a || data[j] == b;
			}
			if (found)
			{
				return base + i;
			}
		}
		return NULL;
	}
}