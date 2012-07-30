CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include (src_list.cmake)
if (WIN32) # Adding the icon to the windows executable
	set(GAME_SRCS ${GAME_SRCS} horus_eye.rc)
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} mac/SDLMain.h mac/SDLMain.m)
ENDIF ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /arch:SSE2 /fp:fast /MP)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_executable(horus_eye ${GAME_SRCS})
target_link_libraries(horus_eye ugdk)
if(WIN32)
    add_custom_command(TARGET horus_eye POST_BUILD 
    COMMAND xcopy externals\\ugdk\\src\\generated\\*.py data\\scripts\\ugdk\\ /q /k /y
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMENT "Python scripts")
else()
    add_custom_command(TARGET horus_eye POST_BUILD 
    COMMAND cp -v externals/ugdk/src/generated/*.py data/scripts/ugdk
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMENT "Python scripts")
endif()

set(GAME_LOCAL_DIR "./data")

CONFIGURE_FILE(
    "game/config.h.in"
    "game/config.h"
)
