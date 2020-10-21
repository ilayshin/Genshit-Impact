#ifndef UTIL
#define UTIL

#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <Psapi.h>
#include <dwmapi.h>
#include <wingdi.h>
#include "cheats.h"

inline DWORD PID = 0;
inline ULONG modBase = 0;
inline HANDLE process_handle;

template<typename T> T RPM(SIZE_T address) {
    T buffer;
    ReadProcessMemory(process_handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
    return buffer;
}

template<typename T> void WPM(SIZE_T address, T buffer) {
    WriteProcessMemory(process_handle, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}

DWORD GetProcId(const char* procName);
MODULEENTRY32 GetModule(DWORD procId, const char* modName);
void* find_pattern(MODULEENTRY32 module, const char* pattern);
void NopEx(void* dst, unsigned int size);

#endif // !UTIL