CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(horus_eye CXX C)
set(horus_eye_VERSION_MAJOR 1)
set(horus_eye_VERSION_MINOR 1)
set(horus_eye_VERSION_SUBMINOR 2)
SET(horus_eye_FULL_VERSION "${horus_eye_VERSION_MAJOR}.${horus_eye_VERSION_MINOR}.${horus_eye_VERSION_SUBMINOR} Alpha")

MESSAGE("== Horus Eye -- version ${horus_eye_FULL_VERSION} ==")

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

set(RESOURCES_VERSION 2)
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources-v${RESOURCES_VERSION}.tgz)
  file(DOWNLOAD http://uspgamedev.org/downloads/projects/horus_eye/dev/resources-v${RESOURCES_VERSION}.tgz ${CMAKE_CURRENT_SOURCE_DIR}/resources-v${RESOURCES_VERSION}.tgz)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf resources-v${RESOURCES_VERSION}.tgz)
endif()

# FindUGDK
option (USE_EMBBEDED_UGDK "" OFF)
find_package(UGDK)
if(UGDK_FOUND AND UGDK_INCLUDE_DIR AND UGDK_LIBRARY AND NOT USE_EMBBEDED_UGDK)
    include_directories(${UGDK_INCLUDE_DIRS})
    set(UGDK_EXTERNAL_LIBRARIES ${UGDK_LIBRARIES})
else()
    add_subdirectory(externals/ugdk)
    include_directories(${UGDK_EXTERNAL_INCLUDE_DIRS})
    set(UGDK_EXTERNAL_LIBRARIES libugdk)
	add_definitions("-DEMBBEDED_UGDK")
endif()

add_subdirectory(src) # The game binary.
