#pragma once
#include "HelperFunctions.hpp"

class Patcher {
	void* _to_patch;
	char* _old_opcodes;
	void* _new_opcodes;
	int _len;
	bool _toggle;

public:
	void enable()
	{
		if (_toggle == true)
			return;

		DWORD oldprotect;
		VirtualProtect(_to_patch, _len, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(_to_patch, _new_opcodes, _len);
		VirtualProtect(_to_patch, _len, oldprotect, &oldprotect);
		_toggle = true;
	}
	void disable()
	{
		if (_toggle == false)
			return;

		if (_old_opcodes != nullptr) {
			DWORD curProtection;
			VirtualProtect(_to_patch, _len, PAGE_EXECUTE_READWRITE, &curProtection);
			// Restore our old bytes
			for (int i = 0; i < _len; ++i) {
				((char*)_to_patch)[i] = _old_opcodes[i];
			}
			VirtualProtect(_to_patch, _len, curProtection, &curProtection);
			_toggle = false;
		}
	}

	Patcher(void* dst, void* src, unsigned int size) : _to_patch(dst), _new_opcodes(src), _old_opcodes(nullptr), _len(size)
	{
		// Save old bytes
		_old_opcodes = (char*)malloc(_len);
		if (_old_opcodes != nullptr) {
			for (int i = 0; i < _len; ++i) {
				_old_opcodes[i] = ((char*)_to_patch)[i];
			}
		}
		_toggle = false;
	}

	~Patcher()
	{
		disable();
		free(_old_opcodes);
	}
};