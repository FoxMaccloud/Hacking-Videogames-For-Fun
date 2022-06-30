#pragma once
#include "pch.h"
#include "Hook.h"
#include "HelperFunctions.h"
#include "Menu.h"

extern Console console;

DWORD64 return_address_send = 0x0;
DWORD counter = 0x0;

SOCKET this_socket_send;
char* this_buffer_send;
DWORD64 this_len_send;
//int this_flags;

float savexmm0;
float savexmm1;
float savexmm2;
float savexmm3;
float savexmm4;
float savexmm5;
float savexmm6;
float savexmm7;
float savexmm8;


inline void update_counter()
{
	if (this_buffer_send)
	{
		counter = *(this_buffer_send + 0x65);
	}
}

inline void print_send()
{
	if (console.log_send)
	{
		std::string output;
		std::string len_output = "Length of package: ";
		int len_package = 0;
		for (DWORD64 i = 0; i < this_len_send; i++)
		{
			output += hex_chars[(this_buffer_send[i] & 0xF0) >> 4];
			output += hex_chars[(this_buffer_send[i] & 0x0F) >> 0];
			//output += " ";
			len_package++;
		}
		len_output += std::to_string(len_package);
		//console.add_log(len_output.c_str());
		console.add_log(output.c_str());
	}
}

void __declspec(naked) send_hook_hk()
{
	// Need to firstly pop rax off the stack since we push it in the hook!
	__asm {
		pop rax;
	}

	__asm {
		mov this_socket_send, rcx
		mov this_buffer_send, rdx
		mov this_len_send, r8
		//mov this_flags, r9
		mov[rsp + 0x8], rbx
		mov[rsp + 0x10], rbp
		mov[rsp + 0x18], rsi
	}

	__asm {
		push rax; save current rax
		push rbx; save current rbx
		push rcx; save current rcx
		push rdx; save current rdx
		push rbp; save current rbp
		push rdi; save current rdi
		push rsi; save current rsi
		push r8; save current r8
		push r9; save current r9
		push r10; save current r10
		push r11; save current r11
		push r12; save current r12
		push r13; save current r13
		push r14; save current r14
		push r15; save current r15
		movss[savexmm0], xmm0
		movss[savexmm1], xmm1
		movss[savexmm2], xmm2
		movss[savexmm3], xmm3
		movss[savexmm4], xmm4
		movss[savexmm5], xmm5
		movss[savexmm6], xmm6
		movss[savexmm7], xmm7
		movss[savexmm8], xmm8
	}

	update_counter();
	print_send();

	__asm {
		movss xmm8, [savexmm8]
		movss xmm7, [savexmm7]
		movss xmm6, [savexmm6]
		movss xmm5, [savexmm5]
		movss xmm4, [savexmm4]
		movss xmm3, [savexmm3]
		movss xmm2, [savexmm2]
		movss xmm1, [savexmm1]
		movss xmm0, [savexmm0]
		pop r15; restore current r15
		pop r14; restore current r14
		pop r13; restore current r13
		pop r12; restore current r12
		pop r11; restore current r11
		pop r10; restore current r10
		pop r9; restore current r9
		pop r8; restore current r8
		pop rsi; restore current rsi
		pop rdi; restore current rdi
		pop rbp; restore current rbp
		pop rdx; restore current rdx
		pop rcx; restore current rcx
		pop rbx; restore current rbx
		pop rax; restore current rax
	}

	__asm {
		jmp[return_address_send]
	}
}