include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(XR-MCU LANGUAGES CXX)

add_executable(XR-MCU ${SOURCES} ${HEADERS})

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)
set(PUBLIC_MODULE_HEADERS ${HEADERS} ${HEADERS_INL} PARENT_SCOPE)

target_include_directories(XR-MCU PUBLIC ${EXTERNAL_DIR})

target_link_libraries(XR-MCU
	${LOCAL_MODULE_DEPENDENCIES}
	${THIRD_PARTY_DEPENDENCIES})

set_target_properties(XR-MCU PROPERTIES FOLDER "MCU")


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	set_target_properties(XR-MCU PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>")
	add_custom_command(TARGET XR-MCU POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONFIG_DIR}/Log4cxxConfig.xml" "${OUTPUT_DIR}/$<CONFIG>")
	add_custom_command(TARGET XR-MCU POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONFIG_DIR}/MCU.ini" "${OUTPUT_DIR}/$<CONFIG>/MCU.ini")
	add_custom_command(TARGET XR-MCU POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${THIRD_PARTY_BINARIES} "${OUTPUT_DIR}/$<CONFIG>")

endif()