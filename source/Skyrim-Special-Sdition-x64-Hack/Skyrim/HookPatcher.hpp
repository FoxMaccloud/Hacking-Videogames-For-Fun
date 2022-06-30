#pragma once
#include <Windows.h>
#include <iostream>
#include "Hook.hpp"

class HookPatcher
{
	void* _to_patch;
	void* _our_func;
	int _len;
	bool _toggle;
	std::unique_ptr<Hook> _hook;
public:

	void enable()
	{
		if (_toggle == true)
			return;
		_hook = std::make_unique<Hook>(_to_patch, _our_func, _len);
		_toggle = true;
	}

	void disable()
	{
		if (_toggle == false || _hook == nullptr)
			return;
		_hook.reset();
		_toggle = false;
	}

	HookPatcher(void* toPatch, void* ourFunc, int len) : _to_patch(toPatch), _our_func(ourFunc), _len(len)
	{
		_toggle = false;
	}

	~HookPatcher()
	{
		_hook.reset();
	}
};