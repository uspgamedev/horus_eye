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

if((NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}) AND (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/.gitignore"))
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND UNIVERSAL)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=10.5")
ENDIF()

if(ANDROID)
    if(${ANDROID_NATIVE_API_LEVEL} LESS 9)
        message(FATAL_ERROR "ANDROID_NATIVE_API_LEVEL must be at least 9")
    endif()
else()
    macro(find_host_package)
        find_package(${ARGN})
    endmacro(find_host_package)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

MESSAGE("== Horus Eye ==")

find_host_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --always
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            OUTPUT_VARIABLE horus_eye_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
                            RESULT_VARIABLE GIT_DESCRIBE_SUCCESS)
    if(NOT ${GIT_DESCRIBE_SUCCESS} EQUAL 0)
        # Source is not in a git repository
        set(horus_eye_VERSION "Unknown version")
    endif()
else(GIT_FOUND)
    set(horus_eye_VERSION "Unknown version")
endif(GIT_FOUND)

set(RESOURCES_VERSION jsonmetadata)
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/resources-${RESOURCES_VERSION}.tar.gz)
    if(GIT_FOUND)
        message("= Downloading resources file version '${RESOURCES_VERSION}'...")
        execute_process(COMMAND ${GIT_EXECUTABLE} archive --remote=git://uspgamedev.org/horus-resources.git
                            -o ${CMAKE_CURRENT_BINARY_DIR}/resources-${RESOURCES_VERSION}.tar.gz ${RESOURCES_VERSION} resources/ )
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_BINARY_DIR}/resources-${RESOURCES_VERSION}.tar.gz
                        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
    else(GIT_FOUND)
        message("-- WARNING: Git not found, unable to download resources.")
    endif(GIT_FOUND)    
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
    
    set(UGDK_BUILD_TESTS    ON CACHE BOOL "Set to ON to enable building tests.")
    set(UGDK_BUILD_EXAMPLES OFF CACHE BOOL "Set to ON to enable building examples.")

    add_definitions(-DEMBBEDED_UGDK="${CMAKE_CURRENT_BINARY_DIR}/externals/ugdk")
    add_subdirectory(externals/ugdk EXCLUDE_FROM_ALL)
    include_directories(${UGDK_EXTERNAL_INCLUDE_DIRS})
    set(UGDK_EXTERNAL_LIBRARIES libugdk)
endif(NOT HORUSEYE_INTERNAL_UGDK)

add_subdirectory(src) # The game binary.

add_ugdk_android_project(horus_eye "Horus Eye" "org.uspgamedev.horuseye")

