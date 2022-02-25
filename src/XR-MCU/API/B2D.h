#pragma once
#ifndef B2D_H
#define B2D_H

#include "../Defines.h"

#include <cstdint>
#include <cstddef> // size_t

#ifdef _WIN32
#ifdef BUILDING_DLL
#define VRT_EXPORT __declspec(dllexport)
#else
#define VRT_EXPORT __declspec(dllimport)
#endif
#else
#define VRT_EXPORT __attribute__((visibility("default")))
#endif

#define VRT_4CC(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

extern "C" {
    // Opaque handle.
    struct vrt_handle;

    // Creates a new packager/streamer and starts the streaming session.
    // @streams: owned by caller
    // The returned pipeline must be freed using vrt_destroy().
    VRT_EXPORT vrt_handle* vrt_create_ext(const char* name, int num_streams, const streamDesc* streams, const char* publish_url = "", int seg_dur_in_ms = 10000, int timeshift_buffer_depth_in_ms = 30000);

    // Deprecated.
    VRT_EXPORT vrt_handle* vrt_create(const char* name, uint32_t MP4_4CC, const char* publish_url = "", int seg_dur_in_ms = 10000, int timeshift_buffer_depth_in_ms = 30000);

    // Destroys a pipeline. This frees all the resources.
    VRT_EXPORT void vrt_destroy(vrt_handle* h);

    // Pushes a buffer to @stream_index. The caller owns it ; the buffer will be copied internally.
    VRT_EXPORT bool vrt_push_buffer_ext(vrt_handle* h, int stream_index, const uint8_t* buffer, const size_t bufferSize);

    // Deprecated.
    VRT_EXPORT bool vrt_push_buffer(vrt_handle* h, const uint8_t* buffer, const size_t bufferSize);

    // Gets the current media time in @timescale unit for @stream_index. Returns -1 on error.
    VRT_EXPORT int64_t vrt_get_media_time_ext(vrt_handle* h, int stream_index, int timescale);

    // Deprecated.
    VRT_EXPORT int64_t vrt_get_media_time(vrt_handle* h, int timescale);
}

#endif // !B2D_H


///*
// * Copyright 2019-2020 Marc Martos.  All rights reserved.
// */
//
//#pragma once
//
// // System includes
//#include <Windows.h>
//#include <cstdint>
//#include <cstddef> // size_t
//
//#define VRT_4CC(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))
//
//// Opaque handle.
//struct vrt_handle;
//
//class B2D {
//public:
//	B2D();
//
//	// Creates a new packager/streamer and starts the streaming session.
//	// @MP4_4CC: codec identifier. Build with VRT_4CC(). For example VRT_4CC('c','w','i','1') for "cwi1".
//	// The returned pipeline must be freed using vrt_destroy().
//	typedef vrt_handle* (*vrt_create)(const char* name, uint32_t MP4_4CC, const char* url, int seg_dur_in_ms, int timeshift_buffer_depth_in_ms);
//	vrt_create create;
//
//	// Destroys a pipeline. This frees all the resources.
//	typedef void(*vrt_destroy)(vrt_handle* h);
//	vrt_destroy destroy;
//
//	// Pushes a buffer. The caller owns it ; the buffer will be copied internally.
//	typedef bool(*vrt_push_buffer)(vrt_handle* h, const uint8_t* buffer, const size_t bufferSize);
//	vrt_push_buffer push_buffer;
//
//	// Gets the current media time in @timescale unit. Returns -1 on error.
//	typedef int64_t(*vrt_get_media_time)(vrt_handle* h, int timescale);
//	vrt_get_media_time get_media_time;
//};
