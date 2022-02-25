
if(NOT SET_UP_CONFIGURATIONS_DONE)
    set(SET_UP_CONFIGURATIONS_DONE 1)

    # No reason to set CMAKE_CONFIGURATION_TYPES if it's not a multiconfig generator
    # Also no reason mess with CMAKE_BUILD_TYPE if it's a multiconfig generator.
    if(CMAKE_CONFIGURATION_TYPES) # multiconfig generator?
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Release_Debug" CACHE STRING "" FORCE) 
    else()
		if(${OptMode} STREQUAL "Debug")
#            	    set(CMAKE_BUILD_TYPE Release_Debug CACHE STRING "" FORCE)
                    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
		elseif(${OptMode} STREQUAL "Release")
		    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
		else()
	            set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
		endif()
    endif()

    # now set up the Release_Debug configuration
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX /std:c++17")
	endif()
	
	#output dirs
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
		set(AGNOSTIC_COPY copy)
		set(AGNOSTIC_RECURSIVE_COPY xcopy /s)
		set(AGNOSTIC_RECURSIVE_REMOVE del /q /s)
		set( CMAKE_HABILITATE_CUDA FALSE )
		
		if(${SYSTEM_ARCHITECTURE} STREQUAL "x86")
			set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/libs/win32")
		elseif(${SYSTEM_ARCHITECTURE} STREQUAL "x64")
			set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/libs/win64")
		else()
			message("Unsupported architecture: ${SYSTEM_ARCHITECTURE}")
		endif()

	elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		ENABLE_LANGUAGE(ASM_NASM)
		set(AGNOSTIC_COPY cp -u)
		set(AGNOSTIC_RECURSIVE_COPY cp -ru)
		set(AGNOSTIC_RECURSIVE_REMOVE rm -rf)
		set(CMAKE_CXX_EXTENSIONS OFF)

		set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/linux64")
	elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		ENABLE_LANGUAGE(ASM_NASM)
		set(AGNOSTIC_COPY cp -u)
		set(AGNOSTIC_RECURSIVE_COPY cp -ru)
		set(AGNOSTIC_RECURSIVE_REMOVE rm -rf)

		set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/libs/iOS")
	else()
		message("Unsupported system: ${CMAKE_SYSTEM_NAME}")
	endif()
endif()
