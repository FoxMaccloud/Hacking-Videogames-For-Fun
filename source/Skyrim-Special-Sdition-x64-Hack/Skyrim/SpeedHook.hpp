#pragma once
#include "HelperFunctions.hpp"
#include "Structures.hpp"
#include "Signatures.hpp"

inline DWORD64 returnAddressSpeed = 0x0;
inline AccelerationBase* currentEntity;

inline _declspec(naked) void speed_h()
{
	__asm {
		pop rax
		mov[currentEntity], rsi;
		
		movss[rsi + 0x000000B4], xmm0;
		mulss xmm2, xmm6;
		mulss xmm2, xmm1;
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

	if (localPlayer->accelerationBase == currentEntity)
	{
		float speedHackMultiplier = cheatVars.runSpeed;
		float forwards = localPlayer->accelerationBase->Forwards;
		float right = localPlayer->accelerationBase->Right;
		if (forwards > 1.0f
			|| forwards < -1.0f
			|| right > 1.0f
			|| right < -1.0f
			)
		{
			localPlayer->accelerationBase->Forwards = forwards * speedHackMultiplier;
			localPlayer->accelerationBase->Right = right * speedHackMultiplier;
		}
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
		jmp[returnAddressSpeed];
	}
}