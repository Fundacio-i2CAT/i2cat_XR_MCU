cmake_minimum_required(VERSION 3.13.0)

project("${SolutionName}")

include("${CMAKE_SOURCE_DIR}/SetupConfig.cmake")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

set(TARGET_DIR "${CMAKE_SOURCE_DIR}/build")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###	SETTING INSTALLED LIBRARIES	###

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	
	# Configure LOG4CXX (bundled library)
	add_library(log4cxx STATIC IMPORTED)
	set_target_properties(log4cxx PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/log4cxx/include"
		IMPORTED_LOCATION "${EXTERNAL_DIR}/log4cxx/lib/liblog4cxx.a")
	target_link_libraries(log4cxx INTERFACE apr aprutil libexpat)

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	add_library(clsocket STATIC IMPORTED)
	set_target_properties(clsocket PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/clsocket/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/clsocket/lib/clsocket_r.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/clsocket/lib/clsocket_d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/clsocket/lib/clsocket_r.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/clsocket/lib/clsocket_d.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(socketio STATIC IMPORTED)
	set_target_properties(socketio PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/socketio/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/socketio/lib/sioclient.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/socketio/lib/sioclient.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/socketio/lib/sioclient.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/socketio/lib/sioclient.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)


	add_library(ddkFramework STATIC IMPORTED)
	set_target_properties(ddkFramework PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/ddk/include;${EXTERNAL_DIR}/ddk/include/ddkFramework"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/ddk/lib/ddkFramework/ddkFramework_r.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/ddk/lib/ddkFramework/ddkFramework_d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/ddk/lib/ddkFramework/ddkFramework_r.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/ddk/lib/ddkFramework/ddkFramework_d.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(ddkUtils STATIC IMPORTED)
	set_target_properties(ddkUtils PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/ddk/include;${EXTERNAL_DIR}/ddk/include/ddkUtils"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/ddk/lib/ddkUtils/ddkUtils_r.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/ddk/lib/ddkUtils/ddkUtils_d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/ddk/lib/ddkUtils/ddkUtils_r.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/ddk/lib/ddkUtils/ddkUtils_d.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(sub SHARED IMPORTED)
	set_target_properties(sub PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/sub/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/sub/lib/signals-unity-bridge.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/sub/lib/signals-unity-bridge.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/sub/lib/signals-unity-bridge.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/sub/lib/signals-unity-bridge.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(b2d SHARED IMPORTED)
	set_target_properties(b2d PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/b2d/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/b2d/lib/bin2dash.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/b2d/lib/bin2dash.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/b2d/lib/bin2dash.lib"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/b2d/lib/bin2dash.lib"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(cwipc SHARED IMPORTED)
	set_target_properties(cwipc PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/cwipc/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/cwipc/lib/cwipc_codec.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/cwipc/lib/cwipc_codec.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/cwipc/lib/cwipc_codec.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/cwipc/lib/cwipc_codec.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(cwipc_util SHARED IMPORTED)
	set_target_properties(cwipc_util PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/cwipc/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/cwipc/lib/cwipc_util.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/cwipc/lib/cwipc_util.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/cwipc/lib/cwipc_util.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/cwipc/lib/cwipc_util.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(pcl_common SHARED IMPORTED)
	set_target_properties(pcl_common PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/pcl/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/pcl/lib/pcl_common_release.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/pcl/lib/pcl_common_release.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/pcl/bin/pcl_common_release.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/pcl/bin/pcl_common_release.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(pcl_io SHARED IMPORTED)
	set_target_properties(pcl_io PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/pcl/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/pcl/lib/pcl_io_release.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/pcl/lib/pcl_io_release.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/pcl/bin/pcl_io_release.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/pcl/bin/pcl_io_release.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	add_library(pcl_io_ply SHARED IMPORTED)
	set_target_properties(pcl_io_ply PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/pcl/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/pcl/lib/pcl_io_ply_release.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/pcl/lib/pcl_io_ply_release.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/pcl/bin/pcl_io_ply_release.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/pcl/bin/pcl_io_ply_release.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)
	
	add_library(pthread SHARED IMPORTED)
	set_target_properties(pthread PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/pthread/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/pthread/lib/pthreadVC3.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/pthread/lib/pthreadVC3d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/pthread/bin/pthreadVC3.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/pthread/bin/pthreadVC3d.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	# Configure log4cxx (bundled library)
	add_library(log4cxx SHARED IMPORTED)
	set_target_properties(log4cxx PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/log4cxx/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/log4cxx/lib/log4cxx.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/log4cxx/lib/log4cxx_d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/log4cxx/bin/log4cxx.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/log4cxx/bin/log4cxx_d.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	include_directories("${EXTERNAL_DIR}/eigen/include")
	include_directories("${EXTERNAL_DIR}/boost/boost-smart-ptr/include;${EXTERNAL_DIR}/boost/boost-config/include;${EXTERNAL_DIR}/boost/boost-assert/include;${EXTERNAL_DIR}/boost/boost-core/include;${EXTERNAL_DIR}/boost/boost-exception/include;${EXTERNAL_DIR}/boost/boost-throw-exception/include;${EXTERNAL_DIR}/boost/boost-mpl/include;${EXTERNAL_DIR}/boost/boost-type-traits/include;${EXTERNAL_DIR}/boost/boost-preprocessor/include;${EXTERNAL_DIR}/boost/boost-utility/include;${EXTERNAL_DIR}/boost/boost-static-assert/include")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

endif()

if(${SolutionName} STREQUAL "Test")
	include("${TEST_DIR}/dependencies.cmake")
else()
	include("${SRC_DIR}/dependencies.cmake")
endif()