#ifndef PATTERN_SCAN_H
#define PATTERN_SCAN_H
#include <Windows.h>
#include <cstdint>
uint64_t FindPattern(const BYTE* lpPattern, LPCSTR szMask);
#endif //PATTERN_SCAN_H
