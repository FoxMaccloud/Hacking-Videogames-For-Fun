#include "Drawing.hpp"
#include "Menu.hpp"

void draw_line(float x, float y, int m_windowWidth, int m_windowHeight)
{
	auto window = ImGui::GetBackgroundDrawList();	
	ImVec2 im_screen = { m_windowWidth/2, m_windowHeight/2 };
	ImVec4 im_color = { 0.4f, 0.07f, 0.07f, 1 };
	window->AddLine({ 0, 0 }, im_screen, ImGui::GetColorU32(im_color), 1.0f);
}

void draw_string(const char* string, float x, float y, ImVec4 im_color)
{
	auto window = ImGui::GetBackgroundDrawList();
	ImVec2 im_screen = { x, y };
	//ImVec4 im_color = { 255, 0, 0, 255 };
	ImVec2 im_size = ImGui::CalcTextSize(string);
	im_screen.x -= im_size.x;
	im_screen.y -= im_size.y;

	window->AddText(nullptr, 0.f, { im_screen.x - 1.f, im_screen.y + 1.f }, ImGui::GetColorU32(IM_COL32_BLACK), string);
	window->AddText(nullptr, 0.f, im_screen, ImGui::GetColorU32(im_color), string);
}

void draw_box(vec2 head, vec2 root)
{
	float top = head.y - 2.0f;
	float bottom = root.y - 8.0f;
	float height = fabs(top - bottom);
	float width = height / 2.0f;
	float left = root.x - width / 2.0f;
	float right = root.x + width / 2.0f;

	auto window = ImGui::GetBackgroundDrawList();
	window->AddRectFilled({ left, bottom }, { right, bottom - height }, ImColor(0, 0, 0, 100));
}
