# - Find UGDK
# This module finds if UGDK is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  UGDK_FOUND           - have the Python libs been found
#  UGDK_LIBRARIES           - path to the python library
#  UGDK_INCLUDE_DIRS        - path to where Python.h is found
#
#=============================================================================
# Copyright (c) 2012 USPGameDev
# 
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
# 
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#   1. The origin of this software must not be misrepresented; you must not
#   claim that you wrote the original software. If you use this software
#   in a product, an acknowledgment in the product documentation would be
#   appreciated but is not required.
#
#   2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.
#
#   3. This notice may not be removed or altered from any source
#   distribution.
#=============================================================================

if(${UGDK_FIND_QUIETLY})
    set(required_status QUIET)
endif(${UGDK_FIND_QUIETLY})
if(${UGDK_FIND_REQUIRED})
    set(required_status ${required_status} REQUIRED)
endif(${UGDK_FIND_REQUIRED})

find_package(OpenGL ${required_status})
find_package(SDL ${required_status})
find_package(SDL_mixer ${required_status})
find_package(SDL_ttf ${required_status})
find_package(SDL_image ${required_status})

# TODO: add info to ugdk/config/config.h if those are necessary
find_package (Lua51 ${required_status})
find_package (PythonLibs 2.6 ${required_status})

FIND_PATH(UGDK_INCLUDE_DIR ugdk
  HINTS ENV UGDK_DIR
  PATH_SUFFIXES ugdk0.4 0.4/include
)

FIND_LIBRARY(UGDK_LIBRARY_DEBUG ugdk0.4-dbg
  HINTS ENV UGDK_DIR
  PATH_SUFFIXES 0.4/lib64/static 0.4/lib/static
)

FIND_LIBRARY(UGDK_LIBRARY ugdk0.4
  HINTS ENV UGDK_DIR
  PATH_SUFFIXES 0.4/lib64/static 0.4/lib/static
)

if(UGDK_LIBRARY AND UGDK_LIBRARY_DEBUG)
    set(UGDK_LIBRARY optimized ${UGDK_LIBRARY} debug ${UGDK_LIBRARY_DEBUG})
endif(UGDK_LIBRARY AND UGDK_LIBRARY_DEBUG)
if(WIN32)
    set(UGDK_LIBRARY ${UGDK_LIBRARY} Ws2_32)
endif(WIN32)

set(UGDK_INCLUDE_DIRS ${UGDK_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})
set(UGDK_LIBRARIES ${UGDK_LIBRARY} ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${LUA_LIBRARIES} ${PYTHON_LIBRARIES})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UGDK REQUIRED_VARS UGDK_LIBRARIES UGDK_INCLUDE_DIRS)

MARK_AS_ADVANCED(UGDK_LIBRARY)
MARK_AS_ADVANCED(UGDK_LIBRARY_DEBUG)
MARK_AS_ADVANCED(UGDK_INCLUDE_DIR)

