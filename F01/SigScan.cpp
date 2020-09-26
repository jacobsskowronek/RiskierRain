#include "SigScan.h"
#include <iostream>
void* SigScan(char* base, size_t size, const char* sig, const char* mask)
{

	size_t sigLength = sizeof(sig);

	for (unsigned int i = 0; i < size - sigLength; i++) {

		bool found = true;

		for (unsigned int j = 0; j < sigLength; j++) {

			if (mask[j] != '?' && sig[j] != *(base + i + j)) {

				found = false;
				break;

			}

		}
		if (found) {

			return (void*)(base + i);

		}

	}

	return nullptr;

}

uintptr_t FindPattern(char* sig, char* mask, uintptr_t offset)
{

	uintptr_t FoundAddress = NULL;

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	uintptr_t pBase = (uintptr_t)sysInfo.lpMinimumApplicationAddress;
	uintptr_t pMax = (uintptr_t)sysInfo.lpMaximumApplicationAddress;

	MEMORY_BASIC_INFORMATION mbi = { 0 };

	while (VirtualQuery((LPCVOID)pBase, &mbi, sizeof(mbi))) {

		if ((mbi.State == MEM_COMMIT) && (mbi.Type == MEM_PRIVATE)) {

			DWORD oldProtect;
			if (!VirtualProtect((LPVOID)pBase, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
				std::cout << "Failed to change page protections: " << GetLastError() << std::endl;
				pBase += mbi.RegionSize;
				continue;
			}

			FoundAddress = (uintptr_t)SigScan((char*)pBase, mbi.RegionSize, sig, mask);

			if (!VirtualProtect((LPVOID)pBase, mbi.RegionSize, oldProtect, &oldProtect)) {
				std::cout << "Failed to revert page protections: " << GetLastError() << std::endl;
				pBase += mbi.RegionSize;
				continue;
			}
			if (FoundAddress) {
				std::cout << "Sig found: " << std::hex << FoundAddress << std::endl;
				FoundAddress += offset;
				break;
			}
		}

		pBase += mbi.RegionSize;
	}

	return FoundAddress;

}
