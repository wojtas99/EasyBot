#include "pattern_scan.h"
#include <windows.h>
#include <psapi.h>
#include <cstdint> // dla uintptr_t

LPVOID FindPattern(const BYTE* lpPattern, LPCSTR szMask)
{
    HMODULE hModule = GetModuleHandle(NULL);
    MODULEINFO moduleInfo = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
        return NULL;

    BYTE* image_base = (BYTE*)moduleInfo.lpBaseOfDll;
    DWORD image_size = moduleInfo.SizeOfImage;
    size_t pattern_length = strlen(szMask);

    BYTE* image_end = image_base + image_size - pattern_length;

    for (BYTE* addr = image_base; addr < image_end; ++addr)
    {
        size_t i = 0;
        for (; i < pattern_length; ++i)
        {
            if (szMask[i] != '?' && addr[i] != lpPattern[i])
                break;
        }
        if (i == pattern_length)
            return addr;
    }
    return NULL;
}
