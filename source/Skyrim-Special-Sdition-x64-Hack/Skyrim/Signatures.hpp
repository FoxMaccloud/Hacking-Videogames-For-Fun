#pragma once
#include "Structures.hpp"
#include "HelperFunctions.hpp"



namespace Signatures
{
	// shared instruction for entities xyz
	inline const char* sharedEnt = "F3 0F ? ? ? F3 44 ? ? ? ? F3 44 ? ? ? ? 8B 47";

	// Stamina/Magic/Health/???
	inline const char* characterStats = "F3 0F ? ? ? 48 8B ? F3 0F ? ? 44 0F ? ? E8";

	// Viewmatrix
	//Address of = SkyrimSE.exe + 0x008B0BAC
	inline const char* viewmatrix = "0F 11 ? ? ? ? ? 0F 10 ? ? 0F 11 ? ? ? ? ? 0F 10 ? ? 0F 11 ? ? ? ? ? 0F 10 ? ? 0F 11 ? ? ? ? ? EB ? 48 89";

	// Writeable coordinates
	//Address of = SkyrimSE.exe + 0x00AAD26F
	inline const char* writeCoords = "0F 29 81 20 01 00 00 4D 8B E1 48 8B 49 20 4C 8D 4C 24 30 48 8B DA 0F 29 B4 24 D0 00 00 00 49 8B 46 10";

	// Write Writeable coordinates
	inline const char* updateWriteCoords = "0F 29 81 20 01 00 00 48 85 C0";

	// Speed Forwards
	// Address of = SkyrimSE.exe + 0x00E014AC
	inline const char* speed = "F3 0F ? ? ? ? ? ? F3 0F ? ? F3 0F ? ? F3 0F ? ? ? ? ? ? 89 BE";
}

namespace Offsets
{
	const		DWORD64 skyrimBase	=	(DWORD64)HelperFunctions::GetModuleInfo(PROCESS).lpBaseOfDll;
	constexpr	DWORD64 localPlayer	=	0x2FC18F8; // 0x02F9B110, 0x01E85EE8, 0x02FC2B68
	constexpr	DWORD64 viewmatrix	=	0x2FE8840;
	constexpr	DWORD64 mapMarkerX	=	0x2FEADF0;
	constexpr	DWORD64 mapMarkerY	=	0x2FEADF4;
	constexpr	DWORD64 mapMarkerZ	=	0x2FEADF8;
}

inline const Entity* localPlayer = *(Entity**)(Offsets::skyrimBase + Offsets::localPlayer);
