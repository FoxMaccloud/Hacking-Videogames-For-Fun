#pragma once
#include "HelperFunctions.h"
#include "Hook.h"
//#include "PacketSorter.h"

class Proxy
{
public:
	Proxy();
	~Proxy();

	void send_package(const char* command);
	void test();


private:
	DWORD64 _module_base;
	
	DWORD64 _send_address;
	Hook* _send_hook;

	DWORD64 _recv_address;
	Hook* _recv_hook;
};