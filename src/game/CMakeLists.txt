SET(GAME_SRCS ./sprites/hero.h ./sprites/floor.h ./sprites/wall.cc ./sprites/floor.cc ./sprites/creature.h ./sprites/worldobject.cc ./sprites/worldobject.h ./sprites/creature.cc ./sprites/wall.h ./main.cc ./utils/levelloader.cc ./utils/levelloader.h ./scenes/world.h ./scenes/world.cc ../framework/engine.cc ../framework/scene.cc ../framework/scene.h ../framework/videomanager.cc ../framework/frame.h ../framework/layer.cc ../framework/videomanager.h ../framework/frame.cc ../framework/inputmanager.cc ../framework/layer.h ../framework/vector2D.h ../framework/types.h ../framework/timeaccumulator.cc ../framework/sprite.cc ../framework/timehandler.cc ../framework/vector2D.cc ../framework/inputmanager.h ../framework/image.h ../framework/timeaccumulator.h ../framework/keys.h ../framework/sprite.h ../framework/engine.h ../framework/timehandler.h ../framework/animation.h ../framework/animation.cc ../framework/image.cc )
#
# Horus Eye
# Copyright (C) 2010  USPGameDev
#
# game/CMakeLists.txt
# Arquivo de configuracao do CMake
#

# Inicializando...
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(horus_eye CXX)
SET(GAME_UNIXNAME horus_eye)
SET(GAME_NAME "Horus Eye")
SET(GAME_VERSION "0.1")
SET(CMAKE_BUILD_TYPE Release)
MESSAGE("${GAME_NAME} versao ${GAME_VERSION}")

# #define's que voce queira incluir na compilacao
# separe por espacos
SET(DEFS "" CACHE STRING "#define's que voce queira incluir na compilacao; separe por espacos")
IF(WIN32)
  SET(DEFS "${DEFS} __WIN32__")
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
  SET(CFLAGS "${CFLAGS} -Wall -ansi -O2 -U_FORTIFY_SOURCE")
  SET(LDFLAGS "${LDFLAGS}")
ELSE(UNIX)
  IF(MINGW)
    # se for o g++ para Windows:
    SET(CFLAGS "${CFLAGS} -Wall -ansi -O2 -U_FORTIFY_SOURCE")
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
FIND_LIBRARY(LSDL NAMES SDL PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDL)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL nao encontrada!")
ENDIF(NOT LSDL)

FIND_LIBRARY(LSDLIMAGE NAMES SDL_image PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDLIMAGE)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL_image nao encontrada!")
ENDIF(NOT LSDLIMAGE)

IF(WIN32)
  FIND_LIBRARY(LSDLMAIN NAMES SDLmain PATH "${CMAKE_LIBRARY_PATH}")
  IF(NOT LSDLMAIN)
    MESSAGE(FATAL_ERROR "Erro fatal: libSDLmain nao encontrada!")
  ENDIF(NOT LSDLMAIN)

  IF(MINGW)
    FIND_LIBRARY(LMINGW32 NAMES mingw32 PATH "${CMAKE_LIBRARY_PATH}")
    IF(NOT LMINGW32)
      MESSAGE(FATAL_ERROR "Erro fatal: libmingw32 nao encontrada!")
    ENDIF(NOT LMINGW32)
  ENDIF(MINGW)
ENDIF(WIN32)

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

# *nix executable
IF(UNIX)
  ADD_EXECUTABLE(${GAME_UNIXNAME} ${GAME_SRCS})
  SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
  TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} m SDL SDL_image)
  SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
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

  # Other properties
  IF(MSVC)
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D__WIN32__ /D__MSVC__ ${CFLAGS} ${CFLAGS_EXTRA}")
    TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} SDLmain SDL SDL_image)
  ELSE(MSVC)

    # RC compiler
    IF(NOT CMAKE_RC_COMPILER)
      SET(CMAKE_RC_COMPILER windres)
    ENDIF(NOT CMAKE_RC_COMPILER)

    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
    TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} m mingw32 SDLmain SDL SDL_image)

    # icone
    #EXECUTE_PROCESS(COMMAND ${CMAKE_RC_COMPILER} -O coff -o src/misc/iconwin.res -i src/iconwin.rc)
    #SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES LINK_FLAGS "src/misc/iconwin.res")
  ENDIF(MSVC)
ENDIF(WIN32)

# Misc
SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES PROJECT_NAME "${GAME_NAME}")

# Installing on *nix
IF(UNIX)
  SET(GAME_INSTALL_DIR "/usr/share/${GAME_UNIXNAME}")
  SET(GAME_FINAL_DIR "/usr/bin")

  INSTALL(CODE "MESSAGE(\"Installing ${GAME_NAME} ${GAME_VERSION}... Make sure you have root privileges.\")")
  INSTALL(TARGETS ${GAME_UNIXNAME} RUNTIME DESTINATION ${GAME_INSTALL_DIR})
  #INSTALL(FILES license.txt readme.html DESTINATION ${GAME_INSTALL_DIR})
  #INSTALL(DIRECTORY config images levels licenses musics quests samples screenshots themes languages DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)
  INSTALL(DIRECTORY data DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)

  INSTALL(CODE "MESSAGE(\"Creating files at ${GAME_FINAL_DIR}...\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"cmake\" \"-E\" \"copy\" \"${GAME_INSTALL_DIR}/${GAME_UNIXNAME}\" \"${GAME_FINAL_DIR}/${GAME_UNIXNAME}\")")

  INSTALL(CODE "MESSAGE(\"Done! Please run ${GAME_UNIXNAME} to start ${GAME_NAME}.\")")
ENDIF(UNIX)
