#pragma once

#ifndef ENTRY
#define ENTRY
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "includes.hpp"
#include "backend/auth/auth.h"

#include "frontend/protection/process/antiDbg.h"

#include "backend/game/world/sdk/cache/caching.hpp"


#include "backend/driver/mouse/mouse.cpp"



auto main() -> void //int main
{
	std::thread(security_loop).detach();

	SonyDriverHelper :: api :: Init();
	
	ClearTempFolder();
	system(_("color D"));
	//auth();
	
	//load_driver();

	vsync();

	utility -> init_driver();


	globals -> width = GetSystemMetrics(SM_CXSCREEN);
	globals -> height = GetSystemMetrics(SM_CYSCREEN);
	
	if (!render->hijack())
	{
		MessageBoxA(0, _("{!} Cheat failed"), _(""), MB_ICONINFORMATION);
		exit(0);
	}

	ShowWindow(GetConsoleWindow(), SW_SHOW);
	caching -> start( );

	
	render -> DirectXInit( );


	caching -> menu( );

	system(_("pause"));
}

#endif
