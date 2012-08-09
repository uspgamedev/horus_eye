CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(horus_eye CXX C)
set(horus_eye_VERSION_MAJOR 1)
set(horus_eye_VERSION_MINOR 1)
set(horus_eye_VERSION_SUBMINOR 2)
SET(horus_eye_FULL_VERSION "${horus_eye_VERSION_MAJOR}.${horus_eye_VERSION_MINOR}.${horus_eye_VERSION_SUBMINOR} Alpha")

MESSAGE("== Horus Eye -- version ${horus_eye_FULL_VERSION} ==")

# TODO: document why we need this.
cmake_policy(SET CMP0015 NEW)

IF (UNIVERSAL)
  # Necessary for a Mac Universal Binary
  SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
  SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
  SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF (UNIVERSAL)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND UNIVERSAL)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=10.5")
ENDIF()

# FindUGDK
add_subdirectory(externals/ugdk)
include_directories(${UGDK_EXTERNAL_INCLUDE_DIRS})

add_subdirectory(src) # The game binary.
