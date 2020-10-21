#include "utils.hpp"

DWORD GetProcId(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_stricmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));

        }
    }
    CloseHandle(hSnap);
    return procId;
}

MODULEENTRY32 GetModule(DWORD procId, const char* modName)
{
    MODULEENTRY32 mod = { 0 };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_stricmp(modEntry.szModule, modName))
                {
                    mod = modEntry;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return mod;
}

void* int_pattern_scan(char* base, size_t size, const char* pattern)
{
    size_t patternLength = strlen(pattern);
    for (size_t i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (int j = 0; j < patternLength; j += 3)
        {
            if (pattern[j] == ' ')
            {
                j -= 2;
                continue;
            }

            if (pattern[j] == '?')
            {
                continue;
            }

            long int  lower = strtol(&pattern[j], 0, 16);

            if ((char)lower != *(char*)(base + i + j / 3))
            {
                found = false; break;
            }
        }
        if (found)
        {
            return (void*)(base + i);
        }
    }
    return nullptr;
}

void* pattern_scan(uintptr_t begin, uintptr_t end, const char* pattern)
{
    uintptr_t currentChunk = begin;
    SIZE_T bytesRead;

    while (currentChunk < end)
    {
        char buffer[4096];

        DWORD oldprotect;
        VirtualProtectEx(process_handle, (void*)currentChunk, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldprotect);
        ReadProcessMemory(process_handle, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
        VirtualProtectEx(process_handle, (void*)currentChunk, sizeof(buffer), oldprotect, &oldprotect);

        if (bytesRead == 0)
        {
            return nullptr;
        }

        void* internalAddress = int_pattern_scan((char*)&buffer, bytesRead, pattern);

        if (internalAddress != nullptr)
        {
            //calculate from internal to external
            uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&buffer;
            return (void*)(currentChunk + offsetFromBuffer);
        }
        else
        {
            //advance to next chunk
            currentChunk = currentChunk + bytesRead;
        }
    }
    return nullptr;
}

void* find_pattern(MODULEENTRY32 module, const char* pattern) {
    uintptr_t begin = (uintptr_t)module.modBaseAddr;
    uintptr_t end = begin + module.modBaseSize;
    return pattern_scan(begin, end, pattern);
}

void NopEx(void* dst, unsigned int size)
{
    byte* nopArray = new byte[size];
    memset(nopArray, 0x90, size);

    DWORD oldprotect;
    VirtualProtectEx(process_handle, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    WriteProcessMemory(process_handle, dst, nopArray, size, NULL);
    VirtualProtectEx(process_handle, dst, size, oldprotect, &oldprotect);
    delete[] nopArray;
}