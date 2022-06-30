#pragma once

#include "HelperFunctions.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Cheats.hpp"

class Menu
{
private:
	std::unique_ptr<Cheats> m_cheats;
	struct CheatMenu
	{
		bool god = false;
		bool esp = false;
		bool fly = false;
		bool speed = false;
	};
	CheatMenu m_menuToggle;
	float m_pos[3] = { 0,0,0 };

#define		GLFW_MOUSE_PASSTHROUGH			0x0002000D

	GLFWwindow* m_window = nullptr;
	WINDOWPLACEMENT m_windowPlacement{};

	HWND m_overlay;
	HWND m_hwnd;
	RECT m_windowSize;
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	int m_imguiWindowWidth = 150;
	int m_imguiWindowHeight = 200;

	bool m_menuShow = false;

	void get_window_size();
	void theme();
	inline void draw_menu();

public:
	Menu();
	~Menu();
	void render();
	void toggle_menu();
};