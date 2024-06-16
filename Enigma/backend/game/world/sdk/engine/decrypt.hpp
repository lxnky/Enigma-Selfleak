#pragma once
#ifndef DECRYPT
#define DECRYPT
#include <iostream>
#include "../../../../../includes.hpp"
#pragma warning(disable : 4996)
namespace cach
{
	class decrypt
	{
	public:

		uintptr_t
			pname,
			weapondata,
			displayname,
			weaponlength;

		std::string
			weaponname;
	};
}

static cach::decrypt* decryptcached = new cach::decrypt();

namespace unrealenginedecryption
{
	class decrypt
	{
	public:

		__forceinline std::string platform(uintptr_t PlayerState)
		{
			std::string result;
			uintptr_t test_platform = ioctl.read<uintptr_t>(PlayerState + 0x438);
			wchar_t platform[64];
			ioctl.read_process(test_platform, platform, sizeof(platform));
			std::wstring platform_wstr(platform);
			std::string platform_str(platform_wstr.begin(), platform_wstr.end());
			if (platform_str == "XBL")
			{
				result = "Xbox";
			}
			else if (platform_str == "PSN")
			{
				result = "PlayStation 4";
			}
			else if (platform_str == "PS5")
			{
				result = "PlayStation 5";
			}
			else if (platform_str == "XSX")
			{
				result = "Xbox Series X";

			}
			else if (platform_str == "SWT")
			{
				result = "Nintendo Switch";
			}
			else if (platform_str == "WIN")
			{
				result = "PC";
			}
			return result;
		}

		__forceinline std::string username(uintptr_t playerstate)
		{
			int pNameLength;
			uint16_t* pNameBufferPointer;
			int i;
			char v25;
			int v26;
			int v29;

			char16_t* pNameBuffer;

			uintptr_t pNameStructure = ioctl.read < uintptr_t >(playerstate + 0xAF0); //next offset 0xAF8
			if (ioctl.is_valid(pNameStructure)) {
				pNameLength = ioctl.read <int>(pNameStructure + 0x10);
				if (pNameLength <= 0)
					return "Bot";

				pNameBuffer = new char16_t[pNameLength];
				uintptr_t pNameEncryptedBuffer = ioctl.read <uintptr_t>(pNameStructure + 0x8);
				if (ioctl.is_valid(pNameEncryptedBuffer)) {
					ioctl.read_process(pNameEncryptedBuffer, pNameBuffer, pNameLength);

					v25 = pNameLength - 1;
					v26 = 0;
					pNameBufferPointer = (uint16_t*)pNameBuffer;

					for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7) {
						v29 = pNameLength - 1;
						if (!(uint32_t)pNameLength)
							v29 = 0;

						if (v26 >= v29)
							break;

						i += 3;
						++v26;
					}
					if (ioctl.is_valid((uint64_t)pNameBuffer))
					{
						std::u16string temp_wstring(pNameBuffer);
						delete[] pNameBuffer;
						return std::string(temp_wstring.begin(), temp_wstring.end());
					}
					else
					{
						return "";
					}
					
				}
			}
		}

		__forceinline std::string weapon(uint64_t CurrentWeapon) {
			std::string PlayersWeaponName = "";

			uint64_t weapondata = ioctl.read<uint64_t>(CurrentWeapon + offsets->weapondata);
			uint64_t ItemName = ioctl.read<uint64_t>(weapondata + 0x38);
			if (!ItemName) return "";

			uint64_t FData = ioctl.read<uint64_t>(ItemName + 0x28);
			int FLength = ioctl.read<int>(ItemName + 0x30);

			if (FLength > 0 && FLength < 50) {

				wchar_t* WeaponBuffer = new wchar_t[FLength];
				ioctl.read_process(FData, (PVOID)WeaponBuffer, FLength * sizeof(wchar_t));
				std::wstring wstr_buf(WeaponBuffer);
				PlayersWeaponName.append(std::string(wstr_buf.begin(), wstr_buf.end()));

				delete[] WeaponBuffer;
			}
			return PlayersWeaponName;
		}

		__forceinline auto reloading() -> bool
		{
			
		}

		__forceinline auto ammocount() -> int
		{
	
		}
	};
}
static unrealenginedecryption::decrypt* decrypt = new unrealenginedecryption::decrypt();
#endif 