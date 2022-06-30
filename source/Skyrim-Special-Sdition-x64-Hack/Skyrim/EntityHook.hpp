#pragma once
#include "HelperFunctions.hpp"
#include "Structures.hpp"

inline DWORD64 returnAddressEntList = 0x0;
inline std::vector<Entity*> g_entities;
inline Entity* ent;

inline float find_distance(vec3 self, vec3 entity)
{
	if (entity.x == NULL || entity.y == NULL || entity.z == NULL)
	{
		return 0.0f;
	}

	float x = (entity.x - self.x);
	float y = (entity.y - self.y);
	float z = (entity.z - self.z);
	float dist = (x * x) + (y * y) + (z * z);
	return (sqrt(dist));
}

inline __declspec(naked) void entity_h()
{
	__asm {
		pop rax;
	}
	__asm {
		mov[ent], rdi
		movss xmm7, [rdi + 0x54]
		movss xmm8, [rdi + 0x58]
		movss xmm9, [rdi + 0x5C]
	}
	__asm {
		push rax
		push rbx
		push rcx
		push rdx
		push rbp
		push rdi
		push rsi
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15
	}

	bool alreadyThere = false;
	if (ent != nullptr
		&& (find_distance(localPlayer->xyz, ent->xyz)/100) < 120.0f)
	{
		for (int i = 0; i < g_entities.size(); i++)
		{
			if (g_entities[i] == ent)
			{
				alreadyThere = true;
			}
		}
		if (!alreadyThere)
		{
			for (int i = 0; i < g_entities.size(); i++)
			{
				if (g_entities.at(i) == nullptr)
				{
					g_entities.at(i) = ent; // insert
					break;
				}
			}
		}
	}

	__asm {
		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop rsi
		pop rdi
		pop rbp
		pop rdx
		pop rcx
		pop rbx
		pop rax
		jmp[returnAddressEntList]
	}
}