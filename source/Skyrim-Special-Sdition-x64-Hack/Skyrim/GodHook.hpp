#pragma once
#include "HelperFunctions.hpp"
#include "Structures.hpp"
#include "Signatures.hpp"

inline DWORD64 returnAddressGod = 0x0;
// 16 bytes
inline _declspec(naked) void god_h()
{
	__asm {
		pop rax
	}

	__asm {
		push rax;
		push rbx;
		push rcx;
		push rdx;
		push rbp;
		push rdi;
		push rsi;
		push r8;
		push r9;
		push r10;
		push r11;
		push r12;
		push r13;
		push r14;
		push r15;
	}

	static Entity* playerCheck = 0x0;
	static float* statChange = 0;

	__asm {
		mov[playerCheck], rbx;
		movss[statChange], xmm6;
	}

	if (localPlayer == playerCheck)
	{
		statChange = 0;
	}

	__asm {
		pop r15;
		pop r14;
		pop r13;
		pop r12;
		pop r11;
		pop r10;
		pop r9;
		pop r8;
		pop rsi;
		pop rdi;
		pop rbp;
		pop rdx;
		pop rcx;
		pop rbx;
		pop rax;
	}

	__asm {
		movss xmm6, [statChange];
		movss[rdi + rbp * 0x4], xmm6;
		mov rcx, rbx;
		subss xmm6, xmm1;
		movaps xmm9, xmm0;
		jmp[returnAddressGod];
	}
}