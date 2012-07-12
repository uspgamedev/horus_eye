CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include (src_list.cmake)
if (WIN32) # Adding the icon to the windows executable
	set(GAME_SRCS ${GAME_SRCS} ../horus_eye.rc)
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} mac/SDLMain.h mac/SDLMain.m)
ENDIF ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -DBOOST_HAS_TR1_TUPLE)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /DBOOST_ALL_NO_LIB /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /arch:SSE2 /fp:fast /MP)
endif ()
if(WIN32)
#    add_definitions(-DWIN32)
endif()

include_directories (".")

find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

find_package(SDL_mixer REQUIRED)
include_directories(${SDLMIXER_INCLUDE_DIR})

add_executable(horus_eye ${GAME_SRCS})
target_link_libraries(horus_eye ugdk)

set(GAME_LOCAL_DIR "./data")

CONFIGURE_FILE(
    "game/config.h.in"
    "game/config.h"
)
