#pragma once
#include "HelperFunctions.hpp"
#include "Structures.hpp"

inline DWORD64 returnAddressCoords = 0x0;
inline PlayerCoordinates* volatile playerCoords = nullptr;

// 14 bytes
inline _declspec(naked) void coordinates_h()
{
	__asm {
		pop rax;
		mov[playerCoords], rcx;
	}

	_asm {
		movaps[rcx + 0x00000120], xmm0;
		mov r12, r9;
		mov rcx, [rcx + 0x20];
		jmp[returnAddressCoords];
	}
}
