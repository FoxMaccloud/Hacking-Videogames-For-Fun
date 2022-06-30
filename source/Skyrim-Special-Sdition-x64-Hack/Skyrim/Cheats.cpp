#include "Cheats.hpp"
#include "Drawing.hpp"

bool validate_entities(int i)
{
	if (g_entities.at(i) == nullptr
		|| g_entities.at(i)->locationPtr == nullptr
		|| g_entities.at(i)->validationPtr1 == nullptr
		|| g_entities.at(i)->validationPtr2 == nullptr
		|| g_entities.at(i)->validationPtr3 == nullptr
		|| ((find_distance(localPlayer->xyz, g_entities.at(i)->xyz)/100) > 150.0f)
		)
	{
		g_entities.at(i) = nullptr;
		return false;
	}
	else
		return true;
}

Cheats::Cheats()
{
	m_entHook		=	std::make_unique<EntityHook>();
	m_godHook		=	std::make_unique<GodHook>();
	m_coordsHook	=	std::make_unique<CoordsHook>();
	m_flyPatch		=	std::make_unique<Patcher>(
		reinterpret_cast<void*>(HelperFunctions::PatternScan(PROCESS, Signatures::updateWriteCoords)),
		(char*)"\x90\x90\x90\x90\x90\x90\x90", 7
		);
	m_speedHook		=	std::make_unique<SpeedHook>();
}

Cheats::~Cheats()
{
}

void Cheats::god_mode(bool active)
{
	if (active)
		m_godHook->enable();
	else
		m_godHook->disable();
}

void Cheats::esp_text(int windowWidth, int windowHeight, bool active)
{
	memcpy(&m_viewmatrix, (PBYTE*)(Offsets::skyrimBase + Offsets::viewmatrix), sizeof(m_viewmatrix));
	for (int i = 0; i < g_entities.size(); i++)
	{
		if (!validate_entities(i))
			return;
		if (!active)
			return;
		
		vec2 vScreen;
		auto pos = g_entities.at(i)->xyz;
		if (HelperFunctions::worldToScreen(pos, vScreen, m_viewmatrix, windowWidth, windowHeight))
		{
			auto s = std::string(g_entities.at(i)->namePtr->name);
			float dist = find_distance(localPlayer->xyz, pos);
			dist = dist / 100;
			s += " [" + std::to_string((int)dist) + "]";
			draw_string(s.c_str(), vScreen.x+32, vScreen.y, cheatVars.espColors);
		}
		
		auto humanoidCheck = std::string(g_entities.at(i)->bonePath1->bonePath2->BoneStructurePtr->humanoidValidation);
		if (humanoidCheck == "NPC COM [COM ]")
		{
			auto head = g_entities.at(i)->bonePath1->bonePath2->BoneStructurePtr->head;
			if (HelperFunctions::worldToScreen(pos, vScreen, m_viewmatrix, windowWidth, windowHeight))
			{
				vec2 vScreenHead;
				if (HelperFunctions::worldToScreen(head, vScreenHead, m_viewmatrix, windowWidth, windowHeight))
				{
					draw_box(vScreenHead, vScreen);
				}
			}
		}
	}
}

void Cheats::teleport(vec3 coordinates)
{
	if (playerCoords != nullptr)
	{
		playerCoords->xyz = coordinates;
	}
}

void Cheats::fly(bool active)
{
	if (!active)
	{
		m_flyPatch->disable();
		return;
	}
	m_flyPatch->enable();
	
	memcpy(&m_viewmatrix, (PBYTE*)(Offsets::skyrimBase + Offsets::viewmatrix), sizeof(m_viewmatrix));
	vec3 newPos = get_player_coords();

	float speed = cheatVars.flySpeed / 10;

	if (GetAsyncKeyState(VK_W))
	{
		newPos.x += m_viewmatrix[8] * speed;
		newPos.y += m_viewmatrix[9] * speed;
		newPos.z += m_viewmatrix[10] * speed;
	}
	if (GetAsyncKeyState(VK_S))
	{
		newPos.x -= m_viewmatrix[8] * speed;
		newPos.y -= m_viewmatrix[9] * speed;
		newPos.z -= m_viewmatrix[10] * speed;
	}
	if (GetAsyncKeyState(VK_D))
	{
		newPos.x += m_viewmatrix[0] * speed;
		newPos.y += m_viewmatrix[1] * speed;
		newPos.z += m_viewmatrix[2] * speed;
	}
	if (GetAsyncKeyState(VK_A))
	{
		newPos.x -= m_viewmatrix[0] * speed;
		newPos.y -= m_viewmatrix[1] * speed;
		newPos.z -= m_viewmatrix[2] * speed;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		newPos.z += speed;
	}
	if (GetAsyncKeyState(VK_CONTROL))
	{
		newPos.z -= speed;
	}
	teleport(newPos);
}

void Cheats::speed(bool active)
{
	if (!active)
	{
		m_speedHook->disable();
		return;
	}
	m_speedHook->enable();
}