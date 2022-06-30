#pragma once
#include "pch.h"
#include "Hook.h"
#include "HelperFunctions.h"
#include "Menu.h"

extern Console console;

DWORD64 return_address_recv = 0x0;

SOCKET this_socket_recv;
char* this_buffer_recv;
DWORD64 this_len_recv;

inline void print_recv()
{
	if (console.log_recv)
	{
		std::string output;
		std::string len_output = "Length of package: ";
		int len_package = 0;
		for (DWORD64 i = 0; i < this_len_recv; i++)
		{
			output += hex_chars[(this_buffer_recv[i] & 0xF0) >> 4];
			output += hex_chars[(this_buffer_recv[i] & 0x0F) >> 0];
			output += " ";
			len_package++;
		}
		len_output += std::to_string(len_package);
		//console.add_log(len_output.c_str());
		console.add_log(output.c_str());
	}
}

void __declspec(naked) recv_hook_hk()
{
	__asm {
		pop rax;
	}
	__asm {
		mov this_socket_recv, rcx
		mov this_buffer_recv, rdx
		mov this_len_recv, r8
		mov[rsp + 0x8], rbx
		mov[rsp + 0x10], rsi
		mov[rsp + 0x20], r9d
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
	}

	print_recv();

	__asm {
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
		jmp[return_address_recv]
	}
}