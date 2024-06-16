#pragma once
#ifndef GLOBALS
#define GLOBALS


namespace globalsdef
{
	class main
	{
	public:
		uintptr_t imagebase;
		uint32_t pid;
		int width, height;
		bool show_menu = true;
		bool visibilitycheck = false;
		bool developermode = false;
		int tabs = 0;
		const char* config_path;
	};

	class aimbot
	{
	public:
		bool enable = true;

		int key = 2;

		int triggerkey = 2;

		bool prediction = false;

		bool fov = false;

		float fovsize = 50;

		float smoothing = 5;

		bool vischeck = false;

		int bone;

		bool triggerbot = false;

		bool playerfreeze = false;

		int triggerspeed = 1.f;

		bool head;
		bool neck;
		bool chest = true;
		bool closest;
	};

	class visual
	{
	public:
		bool enable = true;
		bool box = false;
		int box_type = 0;

		bool cornered = false;
		bool regular = false;
		bool filled = false;
		bool rounded = false;
		bool skeleton = false;
		bool skeleton_outlined = false;
		bool head = false;
		bool outlinedbox = false;
		bool playerdetails = false;
		bool distance = false;
		bool platform = false;
		bool weapon = false;
		bool username = false;
		bool chams = false;
		float box_thickness = 2;
		float skeleton_thickness = 2;
	};

	class colours
	{
	public:
		float visible_color_float[4] = { 0.f, 1.f, 0.f, 1.f };
		float invisible_color_float[4] = { 0.f, 1.f, 0.f, 1.f };
	};

	class exploits
	{
	public:
		bool fovslider = true;
	};

	class misc
	{
	public:
		bool enable_vsync = false;
		int vsync = 1;
		bool crosshair = true;
		bool watermark = true;
		bool fps = false;
		bool show_console = false;
		bool bones = false;
		bool radar = false;
		bool playertp = false;
	};
	class loot
	{
	public:
		bool enable = false;
		bool draw_chests = false;
		bool lamma = false;
		bool pickups = false;
		bool draw_vehicles = false;
		int vehiclesrender = 150;
		int chestrender = 150;
		bool draw_ammo = false;
		int ammo_render = 150;
		int cooler_render = 150;
		bool draw_cooler = false;
		int lammarender = 150;
		int pickupsrender = 150;

		bool Common = true;
		bool rare = true;

		bool purple = true;
		bool gold = true;
		bool mythic = true;
		bool UnCommon = true;
	};
};

static globalsdef::main* globals = new globalsdef::main();
static globalsdef::aimbot* aimbot = new globalsdef::aimbot();
static globalsdef::visual* visuals = new globalsdef::visual();
static globalsdef::colours* colours = new globalsdef::colours();
static globalsdef::exploits* exploits = new globalsdef::exploits();
static globalsdef::misc* misc = new globalsdef::misc();
static globalsdef::loot* loot = new globalsdef::loot();
#endif

