#pragma once
#include "../../includes.hpp"
#include "aimbot/function.hpp"
HWND hwnd;
namespace ut
{
	class utils
	{
	public:
		__forceinline char* wchar_to_char(const wchar_t* pwchar)
		{
			int currentCharIndex = 0;
			char currentChar = pwchar[currentCharIndex];

			while (currentChar != '\0')
			{
				currentCharIndex++;
				currentChar = pwchar[currentCharIndex];
			}

			const int charCount = currentCharIndex + 1;

			char* filePathC = (char*)malloc(sizeof(char) * charCount);

			for (int i = 0; i < charCount; i++)
			{
				char character = pwchar[i];

				*filePathC = character;

				filePathC += sizeof(char);

			}
			filePathC += '\0';

			filePathC -= (sizeof(char) * charCount);

			return filePathC;
		}
		__forceinline DWORD PID(LPCWSTR processName)
		{
			HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			DWORD procID = NULL;

			if (handle == INVALID_HANDLE_VALUE)
				return procID;

			PROCESSENTRY32W entry = { 0 };
			entry.dwSize = sizeof(PROCESSENTRY32W);

			if (Process32FirstW(handle, &entry)) {
				if (!_wcsicmp(processName, entry.szExeFile)) {
					procID = entry.th32ProcessID;
				}
				else while (Process32NextW(handle, &entry)) {
					if (!_wcsicmp(processName, entry.szExeFile)) {
						procID = entry.th32ProcessID;
					}
				}
			}

			CloseHandle(handle);
			return procID;
		}
		void Range(double* x, double* y, float range)
		{
			if (fabs((*x)) > range || fabs((*y)) > range)
			{
				if ((*y) > (*x))
				{
					if ((*y) > -(*x))
					{
						(*x) = range * (*x) / (*y);
						(*y) = range;
					}
					else
					{
						(*y) = -range * (*y) / (*x);
						(*x) = -range;
					}
				}
				else
				{
					if ((*y) > -(*x))
					{
						(*y) = range * (*y) / (*x);
						(*x) = range;
					}
					else
					{
						(*x) = -range * (*x) / (*y);
						(*y) = -range;
					}
				}
			}
		}
		Vector2 RotatePoint(Vector2 radar_pos, Vector2 radar_size, Vector3 LocalLocation, Vector2 TargetLocation) {
			auto dx = TargetLocation.x - LocalLocation.x;
			auto dy = TargetLocation.y - LocalLocation.y;

			auto x = dy * -1;
			x *= -1;
			auto y = dx * -1;

			double calcualted_range = 34 * 1000;

			Range(&x, &y, calcualted_range);

			int rad_x = (int)radar_pos.x;
			int rad_y = (int)radar_pos.y;

			double r_siz_x = radar_size.x;
			double r_siz_y = radar_size.y;

			int x_max = (int)r_siz_x + rad_x - 5;
			int y_max = (int)r_siz_y + rad_y - 5;

			auto return_value = Vector2();

			return_value.x = rad_x + ((int)r_siz_x / 2 + int(x / calcualted_range * r_siz_x));
			return_value.y = rad_y + ((int)r_siz_y / 2 + int(y / calcualted_range * r_siz_y));

			if (return_value.x > x_max)
				return_value.x = x_max;

			if (return_value.x < rad_x)
				return_value.x = rad_x;

			if (return_value.y > y_max)
				return_value.y = y_max;

			if (return_value.y < rad_y)
				return_value.y = rad_y;

			return return_value;
		}
		


		__forceinline std::wstring MBytesToWString(const char* lpcszString)
		{


			int len = strlen(lpcszString);
			int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
			wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
			memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
			::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
			std::wstring wString = (wchar_t*)pUnicode;
			delete[] pUnicode;
			return wString;
		}
		__forceinline std::string WStringToUTF8(const wchar_t* lpwcszWString)
		{


			char* pElementText;
			int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
			pElementText = new char[iTextLen + 1];
			memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
			::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
			std::string strReturn(pElementText);
			delete[] pElementText;
			return strReturn;
		}
		__forceinline void DrawFilledRect(int x, int y, int w, int h, ImU32 color)
		{
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
		}
		void draw_outlined_text(ImVec2 pos, ImColor color, std::string text)
		{
			std::stringstream stream(text);
			std::string line;

			float y = 0.0f;
			int i = 0;

			while (std::getline(stream, line))
			{
				ImVec2 textSize = ImGui::CalcTextSize(line.c_str());

				ImGui::GetBackgroundDrawList()->AddText(ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

				ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), color, line.c_str());

				y = pos.y + textSize.y * (i + 1);
				i++;
			}
		}
		__forceinline void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
		{
			va_list va_alist;
			char buf[1024] = { 0 };
			va_start(va_alist, pText);
			_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
			va_end(va_alist);
			std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
			if (bCenter)
			{
				ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
				x = x - textSize.x / 4;
				y = y - textSize.y;
			}
			if (stroke)
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
			}
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
		}
		__forceinline auto init_driver() -> bool
		{
			system(_("cls"));
			if (!ioctl.start_service())
				return FALSE;

			std::cout << _("\n[i] Waiting For Fortnite...\n");

			while (hwnd == NULL)
			{
				hwnd = FindWindowA(0, _("Fortnite  "));
				Sleep(100);
			}

			
			globals->pid = PID(_(L"FortniteClient-Win64-Shipping.exe"));
			if (!globals->pid)
			{
				system(_("cls"));
				Sleep(3500);
				exit(0);
			}
			if (ioctl.is_mapped(globals->pid)) {

				ioctl.m_base = ioctl.get_module_base(0);

				if (!ioctl.m_base) {
					return FALSE;
				}

				if (!ioctl.resolve_dtb()) {
					return FALSE;
				}
			}
			globals->imagebase = ioctl.m_base;
			
			if (!globals->imagebase)
			{
				system(_("cls"));
				Sleep(3500);
				exit(0);
			}
			system(_("cls"));
			//std::cout << "[i] Process ID -> " << globals->pid << std::endl;;
			//std::cout << "[i] Base Address -> " << globals->imagebase << std::endl;;

			Sleep(2000);

			system(_("cls"));
		}
		
	};
}

static ut::utils* utility = new ut::utils;