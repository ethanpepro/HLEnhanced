# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk

# ~/Library/Application Support/Steam/steamapps/common/Half-Life

cmake_minimum_required(VERSION 3.16)

project(HLEnhanced VERSION 1.0.0 LANGUAGES CXX)

# CMake

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(InputFilesList)

# Sanity?

# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH TRUE)

if(WIN32 AND NOT MSVC)
	message(FATAL_ERROR "Windows must have MSVC compilers installed.")
endif()

if(WIN32 AND CMAKE_CL_64)
	message(FATAL_ERROR "Windows compilers must be 32-bit only.") # TODO: Robust way to ensure 32-bit compilers
endif()

# Platform-specific CMake settings

if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "..." FORCE)
	set(CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}" CACHE PATH "..." FORCE) # TODO: Ensure SDK is 10.13
endif()

# Game installation

set(GAME_DIRECTORY "" CACHE PATH "Path to a valid Steam Half-Life installation.")

if("${GAME_DIRECTORY}" STREQUAL "")
	message(FATAL_ERROR "You must specify a path to a valid Steam Half-Life installation.")
endif()

# Get dependencies

find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

# Get local dependencies

add_library(SDL2 SHARED IMPORTED)

if(WIN32)
	set_target_properties(SDL2 PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/public/SDL2.lib")
elseif(APPLE)
	set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libSDL2.dylib")
else()
	set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libSDL2.so")
endif()

target_include_directories(SDL2 INTERFACE ${CMAKE_SOURCE_DIR}/external/SDL2)

add_library(vgui SHARED IMPORTED)

if(WIN32)
	set_target_properties(vgui PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/public/vgui.lib")
elseif(APPLE)
	set_target_properties(vgui PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/vgui.dylib")
else()
	set_target_properties(vgui PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/vgui.so")
endif()

target_include_directories(vgui INTERFACE ${CMAKE_SOURCE_DIR}/utils/vgui/include)

add_library(tier0 SHARED IMPORTED)

if(WIN32)
	set_target_properties(tier0 PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/public/tier0.lib")
elseif(APPLE)
	set_target_properties(tier0 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libtier0.dylib")
else()
	set_target_properties(tier0 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libtier0.so")
endif()

add_library(vstdlib SHARED IMPORTED)

if(WIN32)
	set_target_properties(vstdlib PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/public/vstdlib.lib")
elseif(APPLE)
	set_target_properties(vstdlib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libvstdlib.dylib")
else()
	set_target_properties(vstdlib PROPERTIES IIMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libvstdlib.so")
endif()

add_library(VTFLib SHARED IMPORTED)

if(WIN32)
	set_target_properties(VTFLib PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/public/VTFLib13.lib")
elseif(APPLE)
	set_target_properties(VTFLib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libVTFLib13.dylib")
else()
	set_target_properties(VTFLib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/public/libVTFLib13.so")
endif()

target_include_directories(VTFLib INTERFACE ${CMAKE_SOURCE_DIR}/external/VTFLib13)

# Cross-project flags

function(set_common_properties TARGET_NAME)
	set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

	if(MSVC)
		set_target_properties(${TARGET_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	target_compile_definitions(${TARGET_NAME} PRIVATE
		$<$<CONFIG:DEBUG>:_DEBUG>

		$<$<PLATFORM_ID:Windows>:
			_CRT_SECURE_NO_WARNINGS
			_SCL_SECURE_NO_WARNINGS>

		$<$<PLATFORM_ID:Linux,Darwin>:
			GNUC
			POSIX
			LINUX
			_LINUX>

		$<$<PLATFORM_ID:Darwin>:
			OSX>

		NO_MALLOC_OVERRIDE)

	target_compile_options(${TARGET_NAME} PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:
			/W4 /WX
			/std:c++14>

		# TODO: Windows math precision
		$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
			-Wall -Werror
			-Wno-overloaded-virtual -Wno-inconsistent-missing-override
			-m32
			-mfpmath=sse -msse2
			-fvisibility=hidden
			-std=c++14>

		$<$<CXX_COMPILER_ID:Clang,AppleClang>:
			-arch i386>

		$<$<PLATFORM_ID:Darwin>:
			-momit-leaf-frame-pointer
			-mtune=core2>)

	target_link_options(${TARGET_NAME} PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:
			/SUBSYSTEM:WINDOWS>

		$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
			-m32>

		$<$<CXX_COMPILER_ID:Clang,AppleClang>:
			-arch i386>)
endfunction()

function(set_common_game_properties TARGET_NAME)
	target_include_directories(${TARGET_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}
		common
		engine
		external
		game/server
		game/shared
		game/shared/entities/player
		game_shared
		pm_shared
		public
		public/math
		shared/engine
		source_sdk/public/tier1
		source_sdk/public
		source_sdk/common)

	target_compile_definitions(${TARGET_NAME} PRIVATE
		CLIENT_WEAPONS)

	target_link_options(${TARGET_NAME} PRIVATE
		$<$<PLATFORM_ID:Linux>:
			-Wl,-Map,${TARGET_NAME}_map.txt>)

	target_link_libraries(${TARGET_NAME} PRIVATE
		$<$<PLATFORM_ID:Windows>:ws2_32>
		$<$<PLATFORM_ID:Linux,Darwin>:dl>

		Threads::Threads

		tier0
		vstdlib
		tier1)
endfunction()

# Shared setup

add_subdirectory(common)
add_subdirectory(engine)
add_subdirectory(game/shared)
add_subdirectory(keyvalues)
add_subdirectory(pm_shared)
add_subdirectory(public)
add_subdirectory(shared/engine)
add_subdirectory(utility)

get_property(SHARED_SRCS GLOBAL PROPERTY SRCS_LIST)

clear_sources()

# Client library

add_subdirectory(source_sdk)

add_sources(${SHARED_SRCS})

add_subdirectory(game/client)
add_subdirectory(shared/engine/client)
add_subdirectory(source_sdk/common)
add_subdirectory(source_sdk/public)
add_subdirectory(vgui_utils)

preprocess_sources()

add_library(client SHARED ${PREP_SRCS})

set_common_properties(client)
set_common_game_properties(client)

target_include_directories(client PRIVATE
	${OPENGL_INCLUDE_DIR}
	game/client
	game/client/ui
	game/client/ui/hud
	game/client/ui/vgui
	shared/engine/client
	vgui_utils)

target_compile_definitions(client PRIVATE
	CLIENT_DLL)

target_link_libraries(client PRIVATE
	SDL2
	OpenGL::GL
	vgui
	vgui2_controls
	VTFLib)

clear_sources()

# Server library

add_sources(${SHARED_SRCS})

add_subdirectory(game/server)

preprocess_sources()

add_library(server SHARED ${PREP_SRCS} server.def)

set_common_properties(server)
set_common_game_properties(server)

target_compile_definitions(server PRIVATE
	SERVER_DLL)

clear_sources()

# Setup project

get_target_property(CLIENT_SOURCES client SOURCES)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${CLIENT_SOURCES})

get_target_property(SERVER_SOURCES server SOURCES)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SERVER_SOURCES})

# Set install targets

install(TARGETS client
	LIBRARY DESTINATION "${GAME_DIRECTORY}/${CMAKE_PROJECT_NAME}/cl_dlls"
	RUNTIME DESTINATION "${GAME_DIRECTORY}/${CMAKE_PROJECT_NAME}/cl_dlls")

install(TARGETS server
	LIBRARY DESTINATION "${GAME_DIRECTORY}/${CMAKE_PROJECT_NAME}/bin"
	RUNTIME DESTINATION "${GAME_DIRECTORY}/${CMAKE_PROJECT_NAME}/bin")
