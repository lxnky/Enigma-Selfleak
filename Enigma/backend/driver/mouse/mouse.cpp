//#pragma once
//
//#include "interception.h"
//#include "../../game/aimbot/movement/mouse.hpp"
//#include <thread>
//#include "../../includes.hpp"
//
//
//InterceptionContext mouse_context;
//InterceptionDevice mouse_device;
//InterceptionStroke mouse_stroke;
//
//
//void mouse::run_mouse_loop( ) {
//	while ( interception_receive( mouse_context, mouse_device = interception_wait( mouse_context ), &mouse_stroke, 1 ) > 0 ) {
//		//std::cout << "in mouse loop\n";
//		if ( interception_is_mouse( mouse_device ) ) {
//			InterceptionMouseStroke& mstroke = *( InterceptionMouseStroke* )&mouse_stroke;
//			interception_send( mouse_context, mouse_device, &mouse_stroke, 1 );
//		}
//	}
//}
//
//bool mouse::initialize_mouse( ) {
//
//	//std::cout << "goofy\n";
//	mouse_context = interception_create_context( );
//	interception_set_filter( mouse_context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE );
//	mouse_device = interception_wait( mouse_context );
//
//	while ( interception_receive( mouse_context, mouse_device = interception_wait( mouse_context ), &mouse_stroke, 1 ) > 0 ) {
//		if ( interception_is_mouse( mouse_device ) ) {
//			
//			InterceptionMouseStroke& mstroke = *( InterceptionMouseStroke* )&mouse_stroke;
//			interception_send( mouse_context, mouse_device, &mouse_stroke, 1 );
//			break;
//		}
//	}
//
//	std::thread( [ & ]( ) { mouse::run_mouse_loop( ); } ).detach( );
//
//	return true;
//}
//
//void mouse::move_mouse( int x, int y ) {
//	InterceptionMouseStroke& mstroke = *( InterceptionMouseStroke* )&mouse_stroke;
//
//	mstroke.flags = 0;
//	mstroke.information = 0;
//	mstroke.x = x;
//	mstroke.y = y;
//
//	interception_send( mouse_context, mouse_device, &mouse_stroke, 1 );
//}
//
//void mouse::mouse_click( ) {
//	InterceptionMouseStroke& mstroke = *( InterceptionMouseStroke* )&mouse_stroke;
//
//	mstroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN;
//	interception_send( mouse_context, mouse_device, &mouse_stroke, 1 );
//
//	mstroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_UP;
//	interception_send( mouse_context, mouse_device, &mouse_stroke, 1 );
//
//	//nsa_import( Sleep )( settings::combat::triggerbot_delay );
//	Sleep(aimbot->triggerspeed);
//}