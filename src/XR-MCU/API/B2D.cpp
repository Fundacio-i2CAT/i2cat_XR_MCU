//#include "B2D.h"
//
//// System includes
//#include <stdio.h>
//
//B2D::B2D() {
//	HINSTANCE hDLL;
//	hDLL = LoadLibrary(L"bin2dash.so"); 
//	if (hDLL != NULL) {
//		create = (vrt_create)GetProcAddress(hDLL, "vrt_create");
//
//		destroy = (vrt_destroy)GetProcAddress(hDLL, "vrt_destroy");
//
//		push_buffer = (vrt_push_buffer)GetProcAddress(hDLL, "vrt_push_buffer");
//
//		get_media_time = (vrt_get_media_time)GetProcAddress(hDLL, "vrt_get_media_time");		
//
//		printf("Bin2Dash DLL Success.\n");
//	} else {
//		printf("¡Error Loading Bin2Dash DLL !\n");
//	}
//}