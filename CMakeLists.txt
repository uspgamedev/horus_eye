CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(horus_eye CXX C)

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
	set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

IF (UNIVERSAL)
  # Necessary for a Mac Universal Binary
  SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
  SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
  SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF (UNIVERSAL)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND UNIVERSAL)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=10.5")
ENDIF()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

find_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --dirty
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            OUTPUT_VARIABLE horus_eye_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
else(GIT_FOUND)
    set(horus_eye_VERSION "Unknown version")
endif(GIT_FOUND)

MESSAGE("== Horus Eye -- ${horus_eye_VERSION} ==")

set(RESOURCES_VERSION 5)
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/resources-v${RESOURCES_VERSION}.tgz)
    message("Downloading resources file version ${RESOURCES_VERSION}...")
    file(DOWNLOAD 
        http://uspgamedev.org/downloads/projects/horus_eye/dev/resources-v${RESOURCES_VERSION}.tar.gz 
        ${CMAKE_CURRENT_BINARY_DIR}/resources-v${RESOURCES_VERSION}.tar.gz)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_BINARY_DIR}/resources-v${RESOURCES_VERSION}.tar.gz
                  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
endif()

# FindUGDK
option(HORUSEYE_INTERNAL_UGDK   "Set to ON to use an internal UGDK instead of the system one." ON)
option(HORUSEYE_FORCE_SUBMODULE "Set to ON to force git submodule update whenever cmake is ran." ON)
if(NOT HORUSEYE_INTERNAL_UGDK)
    find_package(UGDK)
    if(NOT UGDK_FOUND)
        set(HORUSEYE_INTERNAL_UGDK ON CACHE BOOL "Set to ON to use an internal UGDK instead of the system one." FORCE)
        message(FATAL_ERROR "Unable to find UGDK, forcing internal. Please, re-run CMake.")
    endif(NOT UGDK_FOUND)
    include_directories(${UGDK_INCLUDE_DIRS})
    set(UGDK_EXTERNAL_LIBRARIES ${UGDK_LIBRARIES})
    
else(NOT HORUSEYE_INTERNAL_UGDK)
    if(HORUSEYE_FORCE_SUBMODULE OR NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/externals/ugdk/CMakeLists.txt")
        if(GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        else(GIT_FOUND)
            message(FATAL_ERROR "Git submodule 'externals/ugdk' not initialized and git not found.")
        endif(GIT_FOUND)
    endif()
    
    add_definitions(-DEMBBEDED_UGDK="${CMAKE_CURRENT_BINARY_DIR}/externals/ugdk")
    add_subdirectory(externals/ugdk)
    include_directories(${UGDK_EXTERNAL_INCLUDE_DIRS})
    set(UGDK_EXTERNAL_LIBRARIES libugdk)
endif(NOT HORUSEYE_INTERNAL_UGDK)

add_subdirectory(src) # The game binary.
