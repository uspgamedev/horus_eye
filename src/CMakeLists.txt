CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# OPTIONS
option(HORUSEYE_DEBUG_TOOLS "Set to ON to enable debugging tools ingame." ON)


# SOURCE LIST
include (src_list.cmake)
if (WIN32) # Adding the icon to the windows executable
	set(GAME_SRCS ${GAME_SRCS} horus_eye.rc)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -std=gnu++0x)
    if(NOT ANDROID)
        add_definitions(-msse2)
    endif()
endif ()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(
        -Wno-self-assign
        -Wno-deprecated-register
    )
endif ()
if (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
    # SSE2 is automatic in amd64
    IF(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_definitions(/arch:SSE2)
    ENDIF()
endif ()

if(MSVC)
    include_directories(externals/windows/include)
endif(MSVC)
include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_BINARY_DIR})

# FIXME: seriously?
include_directories(SYSTEM ${ugdk_SOURCE_DIR}/externals/glm-0.9.4.2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find Lua and Python
include_directories (${LUA_INCLUDE_DIR})
if(NOT ANDROID)
    include_directories (${PYTHON_INCLUDE_DIR})
endif()

include (module/module_list.cmake)
if(ANDROID)
    set (HORUS_LANGUAGES_LIST "lua")
else()
    set (HORUS_LANGUAGES_LIST "lua;python")
endif()
set (GENERATED_PY_FILES)
include (${CMAKE_SOURCE_DIR}/cmake/SWIG.cmake)
include_directories("${ugdk_SOURCE_DIR}/src") # for SWIG

add_ugdk_executable(horus_eye ${GAME_SRCS} ${GENERATED_SRC})

if(ANDROID)
    set(horus_eye_DATA_PATH "/sdcard/horus_eye")
else()
    set(horus_eye_DATA_PATH "${CMAKE_SOURCE_DIR}/data")
endif()

# Copy the generated .py files to the data/scripts
foreach(it ${GENERATED_PY_FILES} )
    get_filename_component(FILE_NAME ${it} NAME)
    add_custom_command(TARGET horus_eye POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${it} "${horus_eye_DATA_PATH}/scripts/${FILE_NAME}" 
    WORKING_DIRECTORY "../" COMMENT "Copying generated py scripts")
endforeach()

# On windows, copy the .dll files so it can run without annoying anyone.
if(WIN32)
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(LIBS_SOURCE "${CMAKE_SOURCE_DIR}/lib64/")
    ELSE()
        set(LIBS_SOURCE "${CMAKE_SOURCE_DIR}/lib/")
    ENDIF()
    add_custom_command(TARGET horus_eye POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${LIBS_SOURCE}" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying libs from ${LIBS_SOURCE} to the horus_eye's solution directory.")
endif(WIN32)

CONFIGURE_FILE(
    "game/config.h.in"
    "game/config.h"
)
