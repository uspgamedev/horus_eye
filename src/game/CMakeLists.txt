
# Inicializando...
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(horus_eye CXX)
SET(GAME_UNIXNAME horus_eye)
SET(GAME_DEBUGNAME horus_eye_debug)
SET(GAME_NAME "Horus Eye")
SET(GAME_VERSION "1.0.1")
SET(CMAKE_BUILD_TYPE Release)
# Arquivos de código fonte vão aqui!
SET(GAME_SRCS ./scenes/loading.cc ./scenes/pausescene.cc ./scenes/page.h
./scenes/world.h ./scenes/imagescene.h ./scenes/loading.h ./scenes/world.cc
./scenes/menuhandler.h ./scenes/menu.h ./scenes/pagemanager.h
./scenes/scrollingimagescene.cc ./scenes/menu.cc ./scenes/menubuilder.h
./scenes/pagemanager.cc ./scenes/scrollingimagescene.h ./scenes/page.cc
./scenes/menubuilder.cc ./scenes/imagescene.cc ./scenes/pausescene.h ./main.cc
./config.h ./sprites/mummybuilder.h ./sprites/explosion.h ./sprites/item.h
./sprites/fireball.cc ./sprites/pharaoh.cc ./sprites/worldobject.h
./sprites/worldobject.cc ./sprites/itembuilder.cc ./sprites/door.cc
./sprites/magicmissile.cc ./sprites/hero.h ./sprites/conditionbuilder.h
./sprites/condition.h ./sprites/hero.cc ./sprites/wall.h
./sprites/magicmissile.h ./sprites/itemevent.h ./sprites/mummy.h
./sprites/wall.cc ./sprites/fireball.h ./sprites/projectile.cc
./sprites/mummybuilder.cc ./sprites/pharaoh.h ./sprites/item.cc
./sprites/explosion.cc ./sprites/floor.h ./sprites/projectile.h
./sprites/conditionbuilder.cc ./sprites/floor.cc ./sprites/creature.h
./sprites/mummyprojectile.cc ./sprites/door.h ./sprites/weapons/weapon.h
./sprites/weapons/mummyweapon.h ./sprites/weapons/mummyrangedweapon.h
./sprites/weapons/herofireballweapon.h ./sprites/weapons/pharaohrangedweapon.cc
./sprites/weapons/mummyweapon.cc ./sprites/weapons/heroexplosionweapon.h
./sprites/weapons/herobaseweapon.h ./sprites/weapons/heroexplosionweapon.cc
./sprites/weapons/herobaseweapon.cc ./sprites/weapons/herofireballweapon.cc
./sprites/weapons/pharaohsummonweapon.cc ./sprites/weapons/pharaohsummonweapon.h
./sprites/weapons/mummyrangedweapon.cc ./sprites/weapons/pharaohrangedweapon.h
./sprites/creature.cc ./sprites/mummyprojectile.h ./sprites/itembuilder.h
./sprites/mummy.cc ./utils/levelloader.cc ./utils/levelmanager.cc
./utils/rectobject.cc ./utils/hud.cc ./utils/settings.cc
./utils/hudimagefactory.cc ./utils/circleobject.h ./utils/geometryprimitives.h
./utils/textloader.cc ./utils/levelloader.h ./utils/collisionobject.h
./utils/settings.h ./utils/rectobject.h ./utils/tile.cc ./utils/tile.h
./utils/imagefactory.cc ./utils/visionstrategy.h ./utils/textloader.h
./utils/constants.cc ./utils/circleobject.cc ./utils/constants.h
./utils/levelmanager.h ./utils/visionstrategy.cc 
./utils/imagefactory.h ./utils/hudimagefactory.h ./utils/hud.h
./utils/geometryprimitives.cc ../framework/layer.cc ../framework/layer.h
../framework/textmanager.cc ../framework/timeaccumulator.cc ../framework/keys.h
../framework/vector2D.h ../framework/videomanager.h ../framework/textmanager.h
../framework/vector2D.cc ../framework/timehandler.cc ../framework/sprite.cc
../framework/frame.cc ../framework/timehandler.h ../framework/image.cc
../framework/inputmanager.cc ../framework/scene.h ../framework/types.h
../framework/engine.cc ../framework/audiomanager.h ../framework/music.cc
../framework/animation.h ../framework/videomanager.cc ../framework/sample.cc
../framework/light.h ../framework/music.h ../framework/animation.cc
../framework/observer.h ../framework/frame.h ../framework/sprite.h
../framework/scene.cc ../framework/light.cc ../framework/inputmanager.h
../framework/engine.h ../framework/sample.h ../framework/image.h
../framework/audiomanager.cc ../framework/timeaccumulator.h
../framework/pathmanager.cc ../framework/pathmanager.h ../editor/mapeditor.cc
../framework/animationset.h ../framework/animationset.cc ../framework/modifier.h
../framework/modifier.cc ../framework/font.h ../framework/font.cc
../framework/animationparser.h ../framework/animationparser.cc
../editor/mapeditor.h ../editor/mapobject.cc ../editor/mapobject.h
../editor/mapeditor.cc ../editor/scenes/editormenu.h
../editor/scenes/editormenu.cc
../editor/layers/mapspriteslayer.cc ../editor/layers/mapspriteslayer.h
../editor/layers/maptileslayer.cc ../editor/layers/maptileslayer.h
../editor/layers/fpsmeter.cc ../editor/layers/fpsmeter.h ../framework/text.cc
../framework/text.h )

MESSAGE("${GAME_NAME} version ${GAME_VERSION}")

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
  SET(LDFLAGS "${LDFLAGS}")
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
FIND_LIBRARY(LSDL NAMES SDL PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDL)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL nao encontrada!")
ENDIF(NOT LSDL)

FIND_LIBRARY(LSDLIMAGE NAMES SDL_image PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDLIMAGE)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL_image nao encontrada!")
ENDIF(NOT LSDLIMAGE)

FIND_LIBRARY(LSDLMIXER NAMES SDL_mixer PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDLMIXER)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL_mixer nao encontrada!")
ENDIF(NOT LSDLMIXER)

FIND_LIBRARY(LSDLTTF NAMES SDL_ttf PATH "${CMAKE_LIBRARY_PATH}")
IF(NOT LSDLTTF)
  MESSAGE(FATAL_ERROR "Erro fatal: libSDL_ttf nao encontrada!")
ENDIF(NOT LSDLTTF)

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
  TARGET_LINK_LIBRARIES(${GAME_UNIXNAME} m SDL SDL_image SDL_mixer SDL_ttf GL GLU)
  SET_TARGET_PROPERTIES(${GAME_UNIXNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
  # Debug target.
  ADD_EXECUTABLE(${GAME_DEBUGNAME} ${GAME_SRCS})
  SET_TARGET_PROPERTIES(${GAME_DEBUGNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
  TARGET_LINK_LIBRARIES(${GAME_DEBUGNAME} m SDL SDL_image SDL_mixer SDL_ttf GL GLU)
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
    EXECUTE_PROCESS(COMMAND ${CMAKE_RC_COMPILER} -O coff -o ./horus_eye.res -i ./horus_eye.rc)
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
  #INSTALL(DIRECTORY config images levels licenses musics quests samples screenshots themes languages DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)
  INSTALL(DIRECTORY data DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)

  INSTALL(CODE "MESSAGE(\"Creating files at ${GAME_FINAL_DIR}...\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"cmake\" \"-E\" \"copy\" \"${GAME_INSTALL_DIR}/${GAME_UNIXNAME}\" \"${GAME_FINAL_DIR}/${GAME_UNIXNAME}\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"mkdir\" \"$ENV{HOME}/.horus_eye\")")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"chmod\" \"777\" \"$ENV{HOME}/.horus_eye\")")
  INSTALL(CODE "SET(USER_GROUP \"\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"id\" \"-g\" OUTPUT_VARIABLE USER_GROUP)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"chown\" \"$ENV{USER}:${USER_GROUP}\" \"$ENV{HOME}/.horus_eye\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"echo\" \"-n\" \"${GAME_INSTALL_DIR}/\" OUTPUT_FILE \"$ENV{HOME}/.horus_eye/rootpath.txt\")")

  INSTALL(CODE "MESSAGE(\"Done! Please run ${GAME_UNIXNAME} to start ${GAME_NAME}.\")")
ENDIF(UNIX)

SET(USER_HOME "$ENV{HOME}")

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)

