#pragma once
#include <string>
#include <winnt.h>
#include <libloaderapi.h>
#include <time.h>
#include <memoryapi.h>
#include "../encryption/string/xor.hpp"

std::string random_string(size_t length = 32)
{
	std::string result;
	static const std::string chars = _("ABCDEFGHIJKLMNOPQRSTUVXYZabcdefghijklmopqrstuvxyz123456890");
	srand((unsigned)time(0));

	for (size_t i = 0; i < length; i++)
		result += chars[rand() % chars.length()];

	return result;
}

void obfuscator()
{
	//SetConsoleTitleA(random_string().c_str());

	// get our image base
	auto base = reinterpret_cast<uint64_t>(GetModuleHandleA(nullptr));
	if (!base) return;

	// get data structs
	auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
	auto nt = reinterpret_cast<PIMAGE_NT_HEADERS64>(base + dos->e_lfanew);
	auto nt_copy = reinterpret_cast<PIMAGE_NT_HEADERS64>(malloc(sizeof(nt)));

	auto section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < nt->FileHeader.NumberOfSections; i++, section++)
	{
		// wipe section name
		DWORD old;

		VirtualProtect(section, sizeof(*section), PAGE_EXECUTE_READWRITE, &old);

		section->Name[0] = (rand() & 0xff);
		section->Name[1] = '\0';

		section->VirtualAddress = 0;
		section->Misc.PhysicalAddress = 0;
		section->Misc.VirtualSize = 0;

		VirtualProtect(section, sizeof(*section), old, &old);
	}

	// copy NT headers
	memcpy(&nt_copy, &nt, sizeof(nt));
	memset(&nt_copy, 0, sizeof(nt_copy));

	// wipe NT headers
	DWORD old;
	VirtualProtect(&nt, sizeof(nt), PAGE_EXECUTE_READWRITE, &old);
	memcpy(&nt, &nt_copy, sizeof(nt_copy));
	VirtualProtect(&nt, sizeof(nt), old, &old);

}