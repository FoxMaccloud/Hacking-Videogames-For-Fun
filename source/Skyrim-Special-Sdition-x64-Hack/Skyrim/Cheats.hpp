#pragma once
#include "HelperFunctions.hpp"
#include "Hook.hpp"
#include "HookPatcher.hpp"
#include "MemPatcher.hpp"
#include "Signatures.hpp"
#include "Structures.hpp"

#include "EntityHook.hpp"
#include "GodHook.hpp"
#include "CoordsHook.hpp"
#include "SpeedHook.hpp"

class EntityHook
{
public:
	EntityHook()
	{
		g_entities.resize(512);
		m_addrEntHook = (DWORD64)HelperFunctions::PatternScan(PROCESS, Signatures::sharedEnt);
		returnAddressEntList = m_addrEntHook + m_hookSize;
		m_entHook = std::make_unique<Hook>(reinterpret_cast<void*>(m_addrEntHook), reinterpret_cast<void*>(entity_h), m_hookSize);
	};

private:
	std::unique_ptr<Hook> m_entHook;
	DWORD64 m_addrEntHook;
	const int m_hookSize = 17;
};

class GodHook
{
public:
	GodHook()
	{
		m_addrGodHook = (DWORD64)HelperFunctions::PatternScan(PROCESS, Signatures::characterStats);
		returnAddressGod = m_addrGodHook + m_hookSize;
		m_godHook = std::make_unique<HookPatcher>(reinterpret_cast<void*>(m_addrGodHook), reinterpret_cast<void*>(god_h), m_hookSize);
	}
	void enable()
	{
		m_godHook->enable();
	}
	void disable()
	{
		m_godHook->disable();
	}

private:
	std::unique_ptr<HookPatcher> m_godHook;
	DWORD64 m_addrGodHook;
	const int m_hookSize = 16;
};

class CoordsHook
{
public:
	CoordsHook()
	{
		playerCoords = nullptr;
		m_addrCoordsHook = (DWORD64)HelperFunctions::PatternScan(PROCESS, Signatures::writeCoords);
		returnAddressCoords = m_addrCoordsHook + m_hookSize;
		m_coordsHook = std::make_unique<Hook>(reinterpret_cast<void*>(m_addrCoordsHook), reinterpret_cast<void*>(coordinates_h), m_hookSize);
	}

private:
	std::unique_ptr<Hook> m_coordsHook;
	DWORD64 m_addrCoordsHook;
	const int m_hookSize = 14;
};

class SpeedHook
{
public:
	SpeedHook()
	{
		m_addrSpeedHook = (DWORD64)HelperFunctions::PatternScan(PROCESS, Signatures::speed);
		returnAddressSpeed = m_addrSpeedHook + m_hookSize;
		m_speedHook = std::make_unique<HookPatcher>(reinterpret_cast<void*>(m_addrSpeedHook), reinterpret_cast<void*>(speed_h), m_hookSize);
	}
	void enable()
	{
		m_speedHook->enable();
	}
	void disable()
	{
		m_speedHook->disable();
	}
private:
	std::unique_ptr<HookPatcher> m_speedHook;
	DWORD64 m_addrSpeedHook;
	const int m_hookSize = 16;
};

class Cheats
{
private:
	std::unique_ptr<EntityHook> m_entHook;
	std::unique_ptr<GodHook> m_godHook;
	std::unique_ptr<CoordsHook> m_coordsHook;
	std::unique_ptr<Patcher> m_flyPatch;
	std::unique_ptr<SpeedHook> m_speedHook;

	float m_viewmatrix[16];

public:
	Cheats();
	~Cheats();

	void god_mode(bool active);
	void esp_text(int windowWidth, int windowHeight, bool active);
	void teleport(vec3 coordinates);
	void fly(bool active);
	void speed(bool active);

	vec3 get_player_coords(){ if (playerCoords != nullptr) return playerCoords->xyz; else return { 0,0,0 }; };
	vec3 get_mapmarker_coords()
	{		return vec3{ *(float*)(Offsets::skyrimBase + Offsets::mapMarkerX),
					*(float*)(Offsets::skyrimBase + Offsets::mapMarkerY),
					*(float*)(Offsets::skyrimBase + Offsets::mapMarkerZ)};
	}
};
