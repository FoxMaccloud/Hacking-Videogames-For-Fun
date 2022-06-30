#include "Menu.hpp"

void Menu::get_window_size()
{
	GetWindowRect(m_hwnd, &m_windowSize);

	int width = m_windowSize.right - m_windowSize.left - 7;
	int height = m_windowSize.bottom - m_windowSize.top - 7;

	if ((m_windowWidth != width) || (m_windowHeight != height))
	{
		m_windowWidth = width;
		m_windowHeight = height;
		if (m_window)
		{
			glfwSetWindowSize(m_window, m_windowWidth, m_windowHeight);
			GetWindowPlacement(m_hwnd, &m_windowPlacement);
			glfwSetWindowPos(m_window, m_windowPlacement.ptMaxPosition.x, m_windowPlacement.ptMaxPosition.y);
		}
	}
}

inline void Menu::draw_menu()
{
	ImGui::Begin(("Skyrim Hacks by FoxMaccloud"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowSize(ImVec2(400, 200));
		ImGui::Checkbox("ESP", &m_menuToggle.esp);
		ImGui::SameLine(); 
		ImGui::ColorEdit3("ESP Color", (float*)&cheatVars.espColors, ImGuiColorEditFlags_NoInputs);
		ImGui::Checkbox("God", &m_menuToggle.god);
		ImGui::Checkbox("Fly", &m_menuToggle.fly);
		if (m_menuToggle.fly)
			m_menuToggle.god = true;
		ImGui::SameLine(); ImGui::SetCursorPosX(70);
		ImGui::SliderFloat("##", &cheatVars.flySpeed, 1.0f, 30.0f, "Flying Speed = %.1f");

		ImGui::Checkbox("Speed", &m_menuToggle.speed);
		ImGui::SameLine(); ImGui::SetCursorPosX(70);
		ImGui::SliderFloat("###", &cheatVars.runSpeed, 1.0f, 30.0f, "Running Speed = %.1f");

		{
			ImGui::Text("Teleport");
			ImGui::InputFloat3("XYZ", m_pos);

			vec3 mapCoords = m_cheats->get_mapmarker_coords();
			mapCoords.x = mapCoords.x / mapCoordScale;
			mapCoords.y = mapCoords.y / mapCoordScale;
			mapCoords.z = mapCoords.z / mapCoordScale;

			if (ImGui::Button("Go!"))
			{
				m_cheats->teleport(vec3{ m_pos[0], m_pos[1], m_pos[2] });
			} ImGui::SameLine();
			if (ImGui::Button("Go marker!"))
			{
				m_cheats->teleport(mapCoords);
			}
		}
	}
	ImGui::End();
}

Menu::Menu()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	m_hwnd = HelperFunctions::find_main_window();

	get_window_size();

	if (!glfwInit())
		fprintf(stderr, "failed to init!\n");

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int monitorWidth = (int)(glfwGetVideoMode(monitor)->width);
	int monitorHeight = (int)(glfwGetVideoMode(monitor)->height);

	glfwWindowHint(GLFW_FOCUS_ON_SHOW,			GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED,				GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING,				GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE,				GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,GLFW_TRUE);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,		GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED,				GLFW_FALSE);

	m_windowPlacement.length = sizeof(WINDOWPLACEMENT);
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "", NULL, NULL);
	GetWindowPlacement(m_hwnd, &m_windowPlacement);
	glfwSetWindowPos(m_window, m_windowPlacement.ptMaxPosition.x, m_windowPlacement.ptMaxPosition.y);

	if (!m_window)
		glfwTerminate();

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1); // Enable vsync

	if (gl3wInit() != 0)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
	}
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 140"); // glsl_version

	theme();

	m_cheats = std::make_unique<Cheats>();
}

Menu::~Menu()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Menu::render()
{
	get_window_size();

	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_menuShow)
	{
		draw_menu();
	}
	
	m_cheats->god_mode(m_menuToggle.god);
	m_cheats->fly(m_menuToggle.fly);
	m_cheats->speed(m_menuToggle.speed);
	m_cheats->esp_text(m_windowWidth, m_windowHeight, m_menuToggle.esp);

	if (!m_menuShow)
	{
		auto c = m_cheats->get_player_coords();
		m_pos[0] = c.x;
		m_pos[1] = c.y;
		m_pos[2] = c.z;
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_window);
}

void Menu::toggle_menu()
{
	m_menuShow = !m_menuShow;
	if (m_menuShow)
	{
		glfwFocusWindow(m_window);
	}
	else
	{
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetFocus(m_hwnd);
	}
}

void Menu::theme()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->ChildBorderSize = 1;
	style->WindowMinSize = ImVec2(m_imguiWindowHeight, m_imguiWindowWidth);
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->AntiAliasedFill = true;

	style->WindowRounding = 0;
	style->ChildRounding = 0;
	style->FrameRounding = 0;
	style->ScrollbarRounding = 0;
	style->TabRounding = 0;
	style->GrabRounding = 0;
	style->ScrollbarSize = 0.9;

	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);

	style->Colors[ImGuiCol_TitleBg] = ImColor(25, 25, 25, 230);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(25, 25, 25, 230);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(25, 25, 25, 130);

	style->Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 230);
	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(25, 25, 25, 230);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(25, 25, 25, 230);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(25, 25, 25, 230);

	style->Colors[ImGuiCol_Header] = ImColor(35, 35, 35, 230);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(35, 35, 35, 230);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(35, 35, 35, 230);

	style->Colors[ImGuiCol_ResizeGrip] = ImColor(35, 35, 35, 255);
	style->Colors[ImGuiCol_ResizeGripActive] = ImColor(35, 35, 35, 255);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(35, 35, 35, 255);

	style->Colors[ImGuiCol_Button] = ImColor(37, 37, 37, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 41, 41, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(37, 37, 37, 255);

	style->Colors[ImGuiCol_Tab] = ImColor(136, 0, 20, 140);
	style->Colors[ImGuiCol_TabHovered] = ImColor(136, 0, 20, 160);
	style->Colors[ImGuiCol_TabActive] = ImColor(136, 0, 20, 220);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
}