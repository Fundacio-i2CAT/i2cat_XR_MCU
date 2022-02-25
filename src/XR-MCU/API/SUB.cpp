//#include "SUB.h"
//
//// System includes
//#include <stdio.h>
//
//SUB::SUB() {
//	HINSTANCE hDLL;
//	hDLL = LoadLibrary("signals-unity-bridge.dll");
//	if (hDLL != NULL) {
//		create = (sub_create)GetProcAddress(hDLL, "sub_create");
//
//		destroy = (sub_destroy)GetProcAddress(hDLL, "sub_destroy");
//
//		get_stream_count = (sub_get_stream_count)GetProcAddress(hDLL, "sub_get_stream_count");
//
//		get_stream_info = (sub_get_stream_info)GetProcAddress(hDLL, "sub_get_stream_info");
//
//		play = (sub_play)GetProcAddress(hDLL, "sub_play");
//
//		grab_frame = (sub_grab_frame)GetProcAddress(hDLL, "sub_grab_frame");;
//
//		printf("SUB DLL Success.\n");
//	} else {
//		printf("¡Error Loading SUB DLL !\n");
//	}
//}