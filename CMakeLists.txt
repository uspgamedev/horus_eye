
# Inicializando...
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(horus_eye CXX)
SET(GAME_UNIXNAME horus_eye)
SET(GAME_DEBUGNAME horus_eye_debug)
SET(GAME_NAME "Horus Eye")
SET(GAME_VERSION "1.1.1 Alpha")
SET(CMAKE_BUILD_TYPE Release)
# Arquivos de código fonte vão aqui!
include (src/src_list.cmake)
IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} src/mac/SDLMain.h src/mac/SDLMain.m)
ENDIF ()

MESSAGE("${GAME_NAME} version ${GAME_VERSION}")

# Necessary for a Mac Universal Binary
SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
SET (MACOSX_DEPLOYMENT_TARGET 10.5)

# #define's que voce queira incluir na compilacao
# separe por espacos
SET(DEFS "" CACHE STRING "#define's que voce queira incluir na compilacao; separe por espacos")
IF(WIN32)
  SET(DEFS "${DEFS} WIN32")
  IF(MSVC)
    SET(DEFS "${DEFS} _CRT_SECURE_NO_DEPRECATE __MSVC__")
  ENDIF(MSVC)
ENDIF(WIN32)

# CFLAGS: flags que voce queira enviar ao compilador
# LDFLAGS: flags que voce queira enviar ao ligador
SET(CFLAGS "" CACHE STRING "flags enviadas ao compilador")
SET(LDFLAGS "" CACHE STRING "flags enviadas ao linker")
IF(UNIX)
  # se for o g++ para *nix:
  SET(CFLAGS "${CFLAGS} -g -Wall -ansi -O3 -U_FORTIFY_SOURCE")
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(LDFLAGS "${LDFLAGS} -mmacosx-version-min=10.5")
  ELSE()
    SET(LDFLAGS "${LDFLAGS}")
  ENDIF()
ELSE(UNIX)
  IF(MINGW)
    # se for o g++ para Windows:
    SET(CFLAGS "${CFLAGS} -Wall -ansi -O3 -U_FORTIFY_SOURCE")
    SET(LDFLAGS "${LDFLAGS}")
  ELSE(MINGW)
    # se for o Microsoft Visual C++:
    SET(CFLAGS "${CFLAGS}")
    SET(LDFLAGS "${LDFLAGS}")
  ENDIF(MINGW)
ENDIF(UNIX)

# ${GAME_SRCS} esta' definido?
IF(NOT GAME_SRCS)
  MESSAGE(FATAL_ERROR "Erro: GAME_SRCS nao esta' definido! Favor rodar o script ./update_cmake antes do cmake")
ENDIF(NOT GAME_SRCS)

# Encontrando as bibliotecas necessarias:
find_package(OPENGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

find_package(SDL_mixer REQUIRED)
include_directories(${SDLMIXER_INCLUDE_DIR})

find_package(SDL_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})

find_package(SDL_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})

find_library (LUGDK ugdk_d lib/)
if (NOT LUGDK)
    find_library (LUGDK ugdk lib/)
    if (NOT LUGDK)
        message (FATAL_ERROR "Fatal error: libugdk not found!")
    endif (NOT LUGDK)
endif (NOT LUGDK)

# pre-processor: #define's
IF(MSVC)
  FOREACH(d ${DEFS})
    SET(CFLAGS_EXTRA "${CFLAGS_EXTRA} /D${d}")
  ENDFOREACH(d)
ELSE(MSVC)
  FOREACH(d ${DEFS})
    SET(CFLAGS_EXTRA "${CFLAGS_EXTRA} -D${d}")
  ENDFOREACH(d)
ENDIF(MSVC)

cmake_policy(SET CMP0015 NEW)
link_directories ("lib")
include_directories ("include")
include_directories ("src")

# *nix executable
IF(UNIX)
  ADD_EXECUTABLE(${GAME_UNIXNAME} ${GAME_SRCS})
  SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
  TARGET_LINK_LIBRARIES (${GAME_UNIXNAME}
                         m
                         ${LUGDK}
                         ${SDL_LIBRARY}
                         ${SDLMIXER_LIBRARY}
                         ${SDLTTF_LIBRARY}
                         ${SDLIMAGE_LIBRARY}
                         ${OPENGL_LIBRARIES}
                        )
  SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
  # Debug target.
  ADD_EXECUTABLE(${GAME_DEBUGNAME} EXCLUDE_FROM_ALL ${GAME_SRCS})
  SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
  TARGET_LINK_LIBRARIES(${GAME_DEBUGNAME} m ${LUGDK} SDL SDL_image SDL_mixer SDL_ttf GL GLU)
  SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA} -DDEBUG")
ENDIF(UNIX)

# Windows executable
IF(WIN32)

  # MSVC fix
  IF(MSVC)
    STRING(REGEX REPLACE "[.]cc" ".h" GAME_HEADERS "${GAME_SRCS}")
    SET(GAME_SRCS ${GAME_SRCS} ${GAME_HEADERS})
  ENDIF(MSVC)

  # Executables
  ADD_EXECUTABLE(${GAME_UNIXNAME} WIN32 ${GAME_SRCS})
  ADD_EXECUTABLE(${GAME_DEBUGNAME} WIN32 ${GAME_SRCS})

  # Other properties
  IF(MSVC)

    # Release properties
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D__WIN32__ /D__MSVC__ ${CFLAGS} ${CFLAGS_EXTRA}")
    TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} SDLmain SDL SDL_image SDL_mixer SDL_ttf OpenGL32 glu32)
    # Debug properties.
    SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D__WIN32__ /D__MSVC__ ${CFLAGS} ${CFLAGS_EXTRA} -DDEBUG")
    TARGET_LINK_LIBRARIES(${GAME_DEBUGNAME} SDLmain SDL SDL_image SDL_mixer SDL_ttf OpenGL32 glu32)

  ELSE(MSVC)

    # RC compiler
    IF(NOT CMAKE_RC_COMPILER)
      SET(CMAKE_RC_COMPILER windres)
    ENDIF(NOT CMAKE_RC_COMPILER)

    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
    TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} m mingw32 SDLmain SDL SDL_image SDL_mixer SDL_ttf OpenGL32 glu32)
    # Debug.
    SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA} -DDEBUG")
    TARGET_LINK_LIBRARIES(${GAME_DEBUGNAME} m mingw32 SDLmain SDL SDL_image SDL_mixer SDL_ttf OpenGL32 glu32)

    # icone
    EXECUTE_PROCESS(COMMAND ${CMAKE_RC_COMPILER} -O coff -o horus_eye.res -i ./horus_eye.rc)
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "horus_eye.res")
    SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES LINK_FLAGS "horus_eye.res")

  ENDIF(MSVC)

ENDIF(WIN32)

# Misc
SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES PROJECT_NAME "${GAME_NAME}")
SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES PROJECT_NAME "${GAME_NAME} (DEBUG)")

# Installing on *nix
IF(UNIX)
  SET(GAME_INSTALL_DIR "/usr/share/${GAME_UNIXNAME}")
  SET(GAME_FINAL_DIR "/usr/bin")
  
  INSTALL(CODE "MESSAGE(\"Installing ${GAME_NAME} ${GAME_VERSION}... Make sure you have root privileges.\")")
  
  INSTALL(TARGETS ${GAME_UNIXNAME} RUNTIME DESTINATION ${GAME_INSTALL_DIR})
  #INSTALL(FILES license.txt readme.html DESTINATION ${GAME_INSTALL_DIR})
  INSTALL(DIRECTORY data DESTINATION ${GAME_INSTALL_DIR})

  INSTALL(CODE "MESSAGE(\"Done! Please run ${GAME_UNIXNAME} to start ${GAME_NAME}.\")")
ELSE(UNIX)
  SET(GAME_INSTALL_DIR ".")
ENDIF(UNIX)

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/src/game/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/game/config.h"
)

