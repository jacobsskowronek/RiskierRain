#pragma once
#include <Windows.h>

void* SigScan(char* base, size_t size, const char* sig, const char* mask);

uintptr_t FindPattern(char* sig, char* mask, uintptr_t offset);