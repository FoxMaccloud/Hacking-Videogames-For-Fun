#pragma once
#include <imgui.h>
#include "HelperFunctions.hpp"
void draw_line(float x, float y, int m_windowWidth, int m_windowHeight);
void draw_string(const char* string, float x, float y, ImVec4 im_color);
void draw_box(vec2 head, vec2 root);