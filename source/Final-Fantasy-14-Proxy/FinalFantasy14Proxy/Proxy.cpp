#include "pch.h"
#include "Proxy.h"
#include "SendHook.h"
#include "RecvHook.h"
#include "PacketSorter.h"
#include "MemPatcher.h"

#define OFFSET_SEND 0x2320
#define OFFSET_RECV 0x11D90

Proxy::Proxy()
{
	MODULEINFO info = HelperFunctions::GetModuleInfo(L"ffxiv_dx11.exe");
	_module_base = (DWORD64)info.lpBaseOfDll;
	
	MODULEINFO WS2_32 = HelperFunctions::GetModuleInfo(L"WS2_32.dll");
	DWORD64 WS2_32_send = (DWORD64)WS2_32.lpBaseOfDll + OFFSET_SEND;

	_send_address = WS2_32_send;
	int bytes_to_overwrite_send = 15;
	_send_hook = new Hook((void*)_send_address, (void*)send_hook_hk, bytes_to_overwrite_send);
	return_address_send = _send_address + bytes_to_overwrite_send;

	DWORD64 WS2_32_recv = (DWORD64)WS2_32.lpBaseOfDll + OFFSET_RECV;
	_recv_address = WS2_32_recv;
	int bytes_to_overwrite_recv = 15;
	_recv_hook = new Hook((void*)_recv_address, (void*)recv_hook_hk, bytes_to_overwrite_recv);
	return_address_recv = _recv_address + bytes_to_overwrite_recv;
}

Proxy::~Proxy()
{
	_send_hook->~Hook();
	_recv_hook->~Hook();
}

uint64_t get_time_ms()
{
	std::chrono::milliseconds epoch = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch());
	return epoch.count();
}

void Proxy::send_package(const char* command)
{
	int len = strlen(command);
	
	if (len < (int)sizeof(FFXIVARR_PACKET_HEADER))
		return;

	char* send_buffer = new char[len];
	strncpy(send_buffer, command, len);

	size_t i = 0;
	for (size_t count = 0; count < len; ++i, count += 2/*3*/) {
		if (send_buffer[count] >= 'A') {
			send_buffer[count] -= 'A';
			send_buffer[count] += 10;
		}
		else {
			send_buffer[count] -= 48;
		}
		if (send_buffer[count + 1] >= 'A') {
			send_buffer[count + 1] -= 'A';
			send_buffer[count + 1] += 10;
		}
		else {
			send_buffer[count + 1] -= 48;
		}
		send_buffer[i] = (__int8)(((char)send_buffer[count]) * (char)16);
		send_buffer[i] += (__int8)send_buffer[count + 1];
	}
	send_buffer[i] = '\0';
	

	// TODO: Some packets such as "heartbeat packets" do not have timestamps.
	// Looks something like; 00000000000000000000000000000000000000000000000040000000000001000100000000000000180000000000000000000000070000002F040BE09C9ED361
	
	bool timestamp = false;
	for (size_t i = (sizeof(uint64_t)*2); i < (sizeof(uint64_t)*3); i++) // Make this dependent on packet signatures instead of whatever the fuck this is! Also it's broken...
	{
		if (!send_buffer[i] == 0x00)
			timestamp = true;
	}

	if (timestamp)
	{
		uint64_t current_time = get_time_ms();
		//*(uint64_t*)&send_buffer[16] = current_time; // Not sure if this is correct syntax...
		memcpy(&send_buffer[16], &current_time, sizeof(current_time));
	}

	if (counter != NULL)
	{
		counter += 0x1; // Not sure if this works...
		memcpy(&send_buffer[64], &counter, sizeof(counter));
	}

	if (this_socket_send != NULL)
	{
		send(this_socket_send, send_buffer, len/2, NULL);
	}
	else
	{
		console.add_log("[ERROR]: No socket was found!");
	}
	delete[] send_buffer;
}

void Proxy::test()
{
	
}