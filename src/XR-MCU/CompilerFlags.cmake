#add here custom compiler flags for this module

set(THIRD_PARTY_DEPENDENCIES
	log4cxx
	pthread
	ddkFramework
	ddkUtils
	clsocket
	socketio
	sub
	b2d
	cwipc
	cwipc_util
	pcl_common
	pcl_io
	pcl_io_ply)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /Oi /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _USE_MATH_DEFINES /we4834")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} /MD /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _USE_MATH_DEFINES /we4834")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS /D _WINSOCK_DEPRECATED_NO_WARNINGS /D DDK_DEBUG /D _USE_MATH_DEFINES /we4834")

	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/NOLOGO /INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE_DEBUG "/NOLOGO /DEBUG /ASSEMBLYDEBUG")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/NOLOGO /DEBUG /ASSEMBLYDEBUG /NODEFAULTLIB:\"msvcrt.lib\"")

	set(THIRD_PARTY_BINARIES
		"${EXTERNAL_DIR}/log4cxx/bin/log4cxx$<$<CONFIG:Debug>:_d>.dll"
		"${EXTERNAL_DIR}/pthread/bin/pthreadVC3$<$<CONFIG:Debug>:d>.dll"
		"${EXTERNAL_DIR}/sub/bin/signals-unity-bridge.dll"
		"${EXTERNAL_DIR}/sub/bin/libcurl-4.dll"
		"${EXTERNAL_DIR}/sub/bin/libcrypto-44.dll"
		"${EXTERNAL_DIR}/sub/bin/libssl-46.dll"
		"${EXTERNAL_DIR}/sub/bin/AVCC2AnnexBConverter.smd"
		"${EXTERNAL_DIR}/sub/bin/Fmp4Splitter.smd"
		"${EXTERNAL_DIR}/sub/bin/GPACDemuxMP4Full.smd"
		"${EXTERNAL_DIR}/sub/bin/GPACDemuxMP4Simple.smd"
		"${EXTERNAL_DIR}/sub/bin/LibavDemux.smd"
		"${EXTERNAL_DIR}/b2d/bin/bin2dash.dll"
		"${EXTERNAL_DIR}/cwipc/bin/cwipc_codec.dll"
		"${EXTERNAL_DIR}/cwipc/bin/cwipc_util.dll"
		"${EXTERNAL_DIR}/cwipc/bin/OpenNI2.dll"
		"${EXTERNAL_DIR}/pcl/bin/pcl_common_release.dll"
		"${EXTERNAL_DIR}/pcl/bin/pcl_io_ply_release.dll"
		"${EXTERNAL_DIR}/pcl/bin/pcl_io_release.dll"
		"${EXTERNAL_DIR}/libjpeg-turbo/bin/jpeg62.dll"
		"${EXTERNAL_DIR}/libjpeg-turbo/bin/turbojpeg.dll"
		"${EXTERNAL_DIR}/boost/boost-thread/bin/boost_thread-vc141-mt-x64-1_73.dll"
		"${EXTERNAL_DIR}/boost/boost-filesystem/bin/boost_filesystem-vc141-mt-x64-1_73.dll"
		"${EXTERNAL_DIR}/boost/boost-iostreams/bin/boost_iostreams-vc141-mt-x64-1_73.dll"
		"${EXTERNAL_DIR}/bzip2/bin/bz2.dll"
		"${EXTERNAL_DIR}/liblzma/bin/lzma.dll"
		"${EXTERNAL_DIR}/zlib/bin/zlib1.dll"
		"${EXTERNAL_DIR}/zstd/bin/zstd.dll"
		"${EXTERNAL_DIR}/libpng/bin/libpng16.dll"
		"${EXTERNAL_DIR}/b2d/bin/MPEG_DASH.smd"
		"${EXTERNAL_DIR}/b2d/bin/FileSystemSink.smd"
		"${EXTERNAL_DIR}/b2d/bin/GPACMuxMP4.smd"
		"${EXTERNAL_DIR}/b2d/bin/libgpac.dll"
		"${EXTERNAL_DIR}/b2d/bin/libstdc++-6.dll"
		"${EXTERNAL_DIR}/b2d/bin/libcrypto-44.dll"
		"${EXTERNAL_DIR}/b2d/bin/libcurl-4.dll"
		"${EXTERNAL_DIR}/b2d/bin/libgcc_s_seh-1.dll"
		"${EXTERNAL_DIR}/b2d/bin/libssl-46.dll"
		"${EXTERNAL_DIR}/b2d/bin/libwinpthread-1.dll"
		"${EXTERNAL_DIR}/b2d/bin/libzlib.dll"
		"${EXTERNAL_DIR}/b2d/bin/postproc-55.dll"
		"${EXTERNAL_DIR}/b2d/bin/swresample-3.dll"
		"${EXTERNAL_DIR}/b2d/bin/swscale-5.dll")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -D__LINUX__ -D_USE_MATH_DEFINES")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} -g -Wall -std=c++11 -D__LINUX__ -DDDK_DEBUG -D_USE_MATH_DEFINES")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -std=c++11 -D__LINUX__ -DDDK_DEBUG -D_USE_MATH_DEFINES")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

	set(THIRD_PARTY_DEPENDENCIES "")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} -std=c++11 -g -DDDK_DEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -std=c++11 -g -DDDK_DEBUG")

endif()
