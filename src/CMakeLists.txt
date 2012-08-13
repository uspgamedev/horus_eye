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

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

# Find Lua and Python
find_package (Lua51 REQUIRED)
include_directories (${LUA_INCLUDE_DIR})
find_package (PythonLibs 2.6 REQUIRED)
include_directories (${PYTHON_INCLUDE_DIRS})

include (module/module_list.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/SWIG.cmake)
horus_add_scriptlang (lua)
horus_add_scriptlang (python)
set (GENERATED_SRC ${GENERATED_SRC})

add_executable(horus_eye ${GAME_SRCS} ${GENERATED_SRC})
target_link_libraries(horus_eye ${UGDK_EXTERNAL_LIBRARIES})
set(GAME_LOCAL_DIR "./data")

CONFIGURE_FILE(
    "game/config.h.in"
    "game/config.h"
)
