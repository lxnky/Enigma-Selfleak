#include "../../../../../includes.hpp"

namespace cfg
{
	class helper
	{
	public:
		__forceinline BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName) {
			char lpString[1024];
			sprintf_s(lpString, sizeof(lpString), TEXT("%d"), nInteger);
			return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
		}

		__forceinline BOOL WritePrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName) {
			char lpString[1024];
			sprintf_s(lpString, sizeof(lpString), TEXT("%f"), nInteger);
			return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
		}
		__forceinline float GetPrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName)
		{
			char szData[32];

			GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);

			return (float)atof(szData);
		}


	};
	static cfg::helper* help = new cfg::helper();
	class config
	{
	public:
		void savecfg(LPCSTR path)
		{
			help->WritePrivateProfileInt(("a"), TEXT("Aimbot Enbale"), aimbot->enable, path);
			//help->WritePrivateProfileInt(("a"), TEXT("Triggerbot Enbale"), aimbot->triggerbot, path);

			help->WritePrivateProfileInt(("a"), TEXT("Show fov"), aimbot->fov, path);
			help->WritePrivateProfileFloat(("a"), TEXT("fov size"), aimbot->fovsize, path);
			help->WritePrivateProfileFloat(("a"), TEXT("smooth size"), aimbot->smoothing, path);
			help->WritePrivateProfileInt(("a"), TEXT("Aimbot Key"), aimbot->key, path);
			//help->WritePrivateProfileInt(("a"), TEXT("Trigger Key"), hotkeys::trigger_key, path);
			help->WritePrivateProfileInt(("a"), TEXT("Aimbot Bone"), aimbot->bone, path);



			help->WritePrivateProfileInt(("b"), TEXT("Enable visuals"), visuals->enable, path);
			help->WritePrivateProfileInt(("b"), TEXT("Enable boxes"), visuals->box, path);
			//help->WritePrivateProfileInt(("b"), TEXT("Box type"), visuals->box_type, path);
			help->WritePrivateProfileInt(("b"), TEXT("2d box"), visuals->regular, path);
			help->WritePrivateProfileInt(("b"), TEXT("corn box"), visuals->cornered, path);
			help->WritePrivateProfileInt(("b"), TEXT("round box"), visuals->rounded, path);
			help->WritePrivateProfileInt(("b"), TEXT("Skeleton"), visuals->skeleton, path);
			help->WritePrivateProfileInt(("a"), TEXT("head esp"), visuals->head, path);
			help->WritePrivateProfileInt(("b"), TEXT("out Skeleton"), visuals->skeleton_outlined, path);
			help->WritePrivateProfileInt(("b"), TEXT("out box"), visuals->outlinedbox, path);
			//help->WritePrivateProfileInt(("b"), TEXT("Line"), visuals->lines, path);
			help->WritePrivateProfileInt(("b"), TEXT("Distance"), visuals->distance, path);
			help->WritePrivateProfileInt(("b"), TEXT("Weapon"), visuals->weapon, path);
			help->WritePrivateProfileInt(("b"), TEXT("Username"), visuals->username, path);
			help->WritePrivateProfileInt(("b"), TEXT("Platform"), visuals->platform, path);
			help->WritePrivateProfileFloat(("b"), TEXT("Box thick"), visuals->box_thickness, path);
			help->WritePrivateProfileFloat(("b"), TEXT("Skel thick"), visuals->skeleton_thickness, path);

			help->WritePrivateProfileFloat(("b"), TEXT("VisibleColorR"), colours->visible_color_float[0], path);
			help->WritePrivateProfileFloat(("b"), TEXT("VisibleColorG"), colours->visible_color_float[1], path);
			help->WritePrivateProfileFloat(("b"), TEXT("VisibleColorB"), colours->visible_color_float[2], path);

			help->WritePrivateProfileFloat(("b"), TEXT("InvisibleColorR"), colours->invisible_color_float[0], path);
			help->WritePrivateProfileFloat(("b"), TEXT("InvisibleColorG"), colours->invisible_color_float[1], path);
			help->WritePrivateProfileFloat(("b"), TEXT("InvisibleColorB"), colours->invisible_color_float[2], path);




			//help->WritePrivateProfileInt(("c"), TEXT("Streamproof"), misc->streamproof, path);
			//help->WritePrivateProfileInt(("c"), TEXT("Crosshair"), misc->crosshair, path);
			//help->WritePrivateProfileInt(("c"), TEXT("Show Console"), misc->show_console, path);
			help->WritePrivateProfileInt(("c"), TEXT("Show FPS"), misc->fps, path);
		}
		void loadcfg(LPCSTR path)
		{
			aimbot->enable = GetPrivateProfileIntA(("a"), TEXT("Aimbot Enbale"), aimbot->enable, path);
			//aimbot->triggerbot = GetPrivateProfileIntA(("a"), TEXT("Triggerbot Enbale"), aimbot->triggerbot, path);
			aimbot->fov = GetPrivateProfileIntA(("a"), TEXT("Show fov"), aimbot->fov, path);
			aimbot->fovsize = help->GetPrivateProfileFloat(("a"), TEXT("fov size"), aimbot->fovsize, path);
			aimbot->smoothing = help->GetPrivateProfileFloat(("a"), TEXT("smooth size"), aimbot->smoothing, path);
			aimbot->key = GetPrivateProfileIntA(("a"), TEXT("Aimbot Key"), aimbot->key, path);
			//hotkeys::trigger_key = GetPrivateProfileIntA(("a"), TEXT("Trigger Key"), hotkeys::trigger_key, path);
			aimbot->bone = GetPrivateProfileIntA(("a"), TEXT("Aimbot Bone"), aimbot->bone, path);



			visuals->enable = GetPrivateProfileIntA(("b"), TEXT("Enable visuals"), visuals->enable, path);
			visuals->box = GetPrivateProfileIntA(("b"), TEXT("Enable boxes"), visuals->box, path);

			visuals->box_type = GetPrivateProfileIntA(("b"), TEXT("Box type"), visuals->box_type, path);
			visuals->head = GetPrivateProfileIntA(("b"), TEXT("head esp"), visuals->head, path);
			visuals->skeleton = GetPrivateProfileIntA(("b"), TEXT("Skeleton"), visuals->skeleton, path);
			visuals->skeleton_outlined = GetPrivateProfileIntA(("b"), TEXT("out Skeleton"), visuals->skeleton_outlined, path);
			visuals->outlinedbox = GetPrivateProfileIntA(("b"), TEXT("out box"), visuals->outlinedbox, path);
			visuals->rounded = GetPrivateProfileIntA(("b"), TEXT("round box"), visuals->rounded, path);
			visuals->cornered = GetPrivateProfileIntA(("b"), TEXT("corn box"), visuals->cornered, path);
			visuals->regular = GetPrivateProfileIntA(("b"), TEXT("2d box"), visuals->regular, path);
			//visuals->skeleton = GetPrivateProfileIntA(("b"), TEXT("Line"), visuals->lines, path);
			visuals->distance = GetPrivateProfileIntA(("b"), TEXT("Distance"), visuals->distance, path);
			visuals->weapon = GetPrivateProfileIntA(("b"), TEXT("Weapon"), visuals->weapon, path);
			visuals->username = GetPrivateProfileIntA(("b"), TEXT("Username"), visuals->username, path);
			visuals->platform = GetPrivateProfileIntA(("b"), TEXT("Platform"), visuals->platform, path);


			visuals->box_thickness = help->GetPrivateProfileFloat(("b"), TEXT("Box thick"), visuals->box_thickness, path);
			visuals->skeleton_thickness = help->GetPrivateProfileFloat(("b"), TEXT("Skel thick"), visuals->skeleton_thickness, path);


			colours->visible_color_float[0] = help->GetPrivateProfileFloat(("b"), TEXT("VisibleColorR"), colours->visible_color_float[0], path);
			colours->visible_color_float[1] = help->GetPrivateProfileFloat(("b"), TEXT("VisibleColorG"), colours->visible_color_float[1], path);
			colours->visible_color_float[2] = help->GetPrivateProfileFloat(("b"), TEXT("VisibleColorB"), colours->visible_color_float[2], path);

			colours->invisible_color_float[0] = help->GetPrivateProfileFloat(("b"), TEXT("InvisibleColorR"), colours->invisible_color_float[0], path);
			colours->invisible_color_float[1] = help->GetPrivateProfileFloat(("b"), TEXT("InvisibleColorG"), colours->invisible_color_float[1], path);
			colours->invisible_color_float[2] = help->GetPrivateProfileFloat(("b"), TEXT("InvisibleColorB"), colours->invisible_color_float[2], path);


			//misc->streamproof = GetPrivateProfileIntA(("c"), TEXT("Streamproof"), misc->streamproof, path);
			//misc->crosshair = GetPrivateProfileIntA(("c"), TEXT("Crosshair"), misc->crosshair, path);
			//misc->show_console = GetPrivateProfileIntA(("c"), TEXT("Show Console"), misc->show_console, path);
			misc->fps = GetPrivateProfileIntA(("c"), TEXT("Show FPS"), misc->fps, path);
		}
	};
}

static cfg::config* configs = new cfg::config();