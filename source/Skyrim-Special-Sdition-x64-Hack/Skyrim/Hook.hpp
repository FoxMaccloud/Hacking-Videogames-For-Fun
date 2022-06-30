#pragma once
#include <Windows.h>

class Hook {
    void* _to_hook;
    char* _old_opcodes;
    int _len;
public:
    Hook(void* to_hook, void* our_func, int len) : _to_hook(to_hook), _old_opcodes(nullptr), _len(len) {
        // If less then 14 bytes, return.
        if (len < 14) {
            return;
        }

        DWORD curProtection;
        VirtualProtect(to_hook, len, PAGE_EXECUTE_READWRITE, &curProtection);

        // Save old bytes
        _old_opcodes = (char*)malloc(len);
        if (_old_opcodes != nullptr) {
            for (int i = 0; i < len; ++i) {
                _old_opcodes[i] = ((char*)to_hook)[i];
            }
        }

        memset(to_hook, 0x90, len);
        unsigned char patch[] = {
            0x50,                                                           // push     rax
            0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // movabs   rax, 0x0
            0xFF, 0xE0                                                      // jmp      rax
        };
        *(DWORD64*)&patch[3] = (DWORD64)our_func;
        memcpy((void*)to_hook, patch, sizeof(patch));

        VirtualProtect(to_hook, len, curProtection, &curProtection);
    }

    ~Hook() {
        if (_old_opcodes != nullptr) {
            DWORD curProtection;
            VirtualProtect(_to_hook, _len, PAGE_EXECUTE_READWRITE, &curProtection);
            // Restore our old bytes
            for (int i = 0; i < _len; ++i) {
                ((char*)_to_hook)[i] = _old_opcodes[i];
            }
            VirtualProtect(_to_hook, _len, curProtection, &curProtection);
            free(_old_opcodes);
        }
    }
};
