#pragma once
#include "pch.h"
#include "kiero.h"
#include "HelperFunctions.h"
#include "Proxy.h"


/*
I'm not sure why, but for some reason ImGui doesn't like it when I use pre-compiled headers...
I therefore made a hmenu as a backup.
*/

#define WINDOW_NAME "Final Fantasy XIV Proxy"

#define KIERO_INCLUDE_D3D9   0 // 1 if you need D3D9 hook
#define KIERO_INCLUDE_D3D10  0 // 1 if you need D3D10 hook
#define KIERO_INCLUDE_D3D11  1 // 1 if you need D3D11 hook
#define KIERO_INCLUDE_D3D12  0 // 1 if you need D3D12 hook
#define KIERO_INCLUDE_OPENGL 0 // 1 if you need OpenGL hook
#define KIERO_INCLUDE_VULKAN 0 // 1 if you need Vulkan hook

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

extern Proxy proxy;

extern bool menu_show_e;
extern bool lock_inputs_e;

namespace Menu
{
	void start();
	void Theme();
	void stop();
}

struct Console
{
	char                  input_buffer[16384];
	ImVector<char*>       items;				// Should clear the items that are older then e.g., 1000 packages. Having a huge amount of items gives performance issues as it has to render them all on screen!
	int					  max_log_size = 5;
	ImVector<const char*> commands;
	ImVector<char*>       history;
	int                   history_pos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       filter;
	bool                  auto_scroll;
	bool                  scroll_to_bottom;
	bool				  copy_to_clipboard;

	bool log_send = false;
	bool log_recv = false;

	ImVector<char*>	timestamps;

	Console()
	{
		clear_log();
		memset(input_buffer, 0, sizeof(input_buffer));
		history_pos = -1;

		// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
		commands.push_back("help");
		commands.push_back("history");
		commands.push_back("clear");
		auto_scroll = true;
		scroll_to_bottom = false;
		add_log("Welcome!");
	}
	~Console()
	{
		clear_log();
		for (int i = 0; i < history.Size; i++)
			free(history[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
	static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

	void clear_log()
	{
		for (int i = 0; i < items.Size; i++) {
			free(items[i]);
		}
		for (int i = 0; i < timestamps.Size; i++)
		{
			free(timestamps[i]);
		}
		items.clear();
		timestamps.clear();
	}

	void add_log(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[16384];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		items.push_back(Strdup(buf));

		char time_buffer[16];
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::strftime(time_buffer, sizeof(time_buffer), "%H.%M.%S", std::localtime(&now));
		timestamps.push_back(Strdup(time_buffer));

		// How the fuck do I implement this shit...
		//if (items.size() >= max_log_size)
		//{
		//	free(items[items.size()-1]);
		//	items.pop_back();
		//}
	}

	// Move me to packetsorter!
	// Mans gotta learn structs and where to find them opcodes my guy.
	std::string check_opcode(std::string opcode)
	{
		if (opcode == "6310") //03E0 // 0315 / 1072?
			return "HEARTBEAT";
		else if (opcode == "1B02") // 0147
			return "MOVEMENT";
		else if (opcode == "4D03")
			return "CAST";
		else if (opcode == "E700")
			return "POSE";
		else
			return "UNKNOWN";
	}

	std::string bytes_to_two_byte(std::string opcode)
	{
		std::string two_byte;
		for (int i = opcode.length()-1; i > 0; i-=2) 
		{
			two_byte += opcode[i - 1];
			two_byte += opcode[i];
		}
		return two_byte;
	}	

	void Draw()
	{
		ImGui::Checkbox("Log Send", &log_send);
		ImGui::SameLine(); ImGui::Checkbox("Log Recv", &log_recv);
		ImGui::SameLine(); ImGui::Checkbox("Auto-scroll", &auto_scroll);
		ImGui::SameLine(); copy_to_clipboard = ImGui::Button("Copy output", ImVec2(90, 19));
		ImGui::SameLine(); if (ImGui::Button("Clear", ImVec2(45, 19))) { clear_log(); };
		ImGui::SameLine(); filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 100.0f); ImGui::Checkbox("Lock input?", &lock_inputs_e);
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + (ImGui::GetFrameHeightWithSpacing() * 2);
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_NoScrollbar /*ImGuiWindowFlags_HorizontalScrollbar*/ | ImGuiWindowFlags_NoScrollWithMouse);
		{
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) clear_log();
				ImGui::EndPopup();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			if (copy_to_clipboard)
				ImGui::LogToClipboard();

			ImGui::BeginTable("Packets", 5, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY);
			{
				ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0.08f);
				ImGui::TableSetupColumn("Opcode", ImGuiTableColumnFlags_None, 0.08f);
				ImGui::TableSetupColumn("Packet", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_None, 0.05f);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_None, 0.04f);
				ImGui::TableHeadersRow();

				for (int i = 0; i < items.Size; i++)
				{

					const char* item = items[i];
					if (!filter.PassFilter(item))
						continue;

					std::string opcode;
					bool self_sent = false;

					if (strncmp(item, "# ", 2) == 0)
					{
						opcode = std::string(item + 116 + 2, 4);
						self_sent = true;
					}
					else
					{
						opcode = std::string(item + 116, 4);
					}

					// strstr() doesn't work for some weird reason... Doesn't work below either...
					if (strstr(item, "[error]"))
					{
						opcode = "";
					}

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);

					ImGui::TextWrapped(check_opcode(opcode).c_str());
					
					ImGui::TableSetColumnIndex(1);

					// Gets the opcode assuming it's at a fixed length
					
					ImGui::TextWrapped(bytes_to_two_byte(opcode).c_str());

					ImGui::TableSetColumnIndex(2);

					ImVec4 color;
					bool has_color = false;
					if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
					else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
					if (has_color)
						ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextWrapped(item);
					if (has_color)
						ImGui::PopStyleColor();

					ImGui::TableSetColumnIndex(3);

					int packet_length = strlen(item);
					if (self_sent)
					{
						packet_length -= 2;
					}
					ImGui::TextWrapped(std::to_string(packet_length/2).c_str());

					ImGui::TableSetColumnIndex(4);

					const char* timestamp = timestamps[i];
					ImGui::TextWrapped(timestamp);

					// TODO: Fix autoscroll for Table.

				}
				if (copy_to_clipboard)
					ImGui::LogFinish();
				if (scroll_to_bottom || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY(1.0f);
				scroll_to_bottom = false;
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();

		ImGui::Separator();
		
		bool reclaim_focus = false;

		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 60);
		{
			//if (ImGui::Button("Clear", ImVec2(45, 20))) { 
			//	char* s = input_buffer;
			//	Strtrim(s);
			//	strcpy(input_buffer, "");
			//	reclaim_focus = true;
			//};
			if (ImGui::Button("Send", ImVec2(45, 40))) {
				char* s = input_buffer;
				Strtrim(s);
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			};
		}
		ImGui::NextColumn();
		{
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CtrlEnterForNewLine;
			if (ImGui::InputTextMultiline("Input", input_buffer, IM_ARRAYSIZE(input_buffer), ImVec2(ImGui::GetWindowSize().x / 3, ImGui::GetWindowSize().y / 10), input_text_flags, NULL, NULL))
			{
				char* s = input_buffer;
				Strtrim(s);				
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			}
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
		}
	}

	void ExecCommand(const char* command_line)
	{
		add_log("# %s\n", command_line);

		// Insert into history. First find match and delete it so it can be pushed to the back.
		// This isn't trying to be smart or optimal.
		history_pos = -1;
		for (int i = history.Size - 1; i >= 0; i--)
			if (Stricmp(history[i], command_line) == 0)
			{
				free(history[i]);
				history.erase(history.begin() + i);
				break;
			}
		history.push_back(Strdup(command_line));

		// Process command
		if (Stricmp(command_line, "CLEAR") == 0)
		{
			clear_log();
		}
		else if (Stricmp(command_line, "HELP") == 0)
		{
			add_log("Commands:");
			for (int i = 0; i < commands.Size; i++)
				add_log("- %s", commands[i]);
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = history.Size - 10;
			for (int i = first > 0 ? first : 0; i < history.Size; i++)
				add_log("%3d: %s\n", i, history[i]);
		}
		else
		{
			proxy.send_package(command_line);
		}
		scroll_to_bottom = true;
	}
};