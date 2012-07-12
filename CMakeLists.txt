CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(horus_eye CXX C)
SET(GAME_UNIXNAME horus_eye)
SET(GAME_NAME "Horus Eye")
SET(GAME_VERSION "1.1.2 Alpha")
SET(CMAKE_BUILD_TYPE Release)

include (src/src_list.cmake)
if (WIN32) # Adding the icon to the windows executable
	set(GAME_SRCS ${GAME_SRCS} horus_eye.rc)
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} src/mac/SDLMain.h src/mac/SDLMain.m)
ENDIF ()

MESSAGE("== ${GAME_NAME} version ${GAME_VERSION} ==")

IF (UNIVERSAL)
  # Necessary for a Mac Universal Binary
  SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
  SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
  SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF (UNIVERSAL)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -DBOOST_HAS_TR1_TUPLE)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /DBOOST_ALL_NO_LIB /DBOOST_HAS_TR1_TUPLE /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /arch:SSE2 /fp:fast /MP)
endif ()
if(WIN32)
    add_definitions(-DWIN32)
endif()

# LDFLAGS: flags que voce queira enviar ao ligador
SET(LDFLAGS "" CACHE STRING "flags enviadas ao linker")

# se for o g++ para *nix:
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND UNIVERSAL)
    SET(LDFLAGS "${LDFLAGS} -mmacosx-version-min=10.5")
ENDIF()

subdirs(externals/ugdk)

cmake_policy(SET CMP0015 NEW)
include_directories ("include")
include_directories ("src")
include_directories ("externals/sdl1.2/include")
include_directories ("externals/ugdk/src")

ADD_EXECUTABLE(${GAME_UNIXNAME} ${GAME_SRCS})
SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES PROJECT_NAME "${GAME_NAME}")
TARGET_LINK_LIBRARIES (${GAME_UNIXNAME} ugdk)

CONFIGURE_FILE(
    "src/game/config.h.in"
    "src/game/config.h"
)
