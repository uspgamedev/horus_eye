
# Inicializando...
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(horus_eye CXX)
SET(GAME_UNIXNAME horus_eye)
SET(GAME_DEBUGNAME horus_eye_debug)
SET(GAME_NAME "Horus Eye")
SET(GAME_VERSION "1.1.0")
SET(CMAKE_BUILD_TYPE Release)
# Arquivos de código fonte vão aqui!
SET(GAME_SRCS ./src/editor/layers/fpsmeter.cc ./src/editor/layers/fpsmeter.h ./src/editor/layers/mapspriteslayer.cc ./src/editor/layers/mapspriteslayer.h ./src/editor/layers/maptileslayer.cc ./src/editor/layers/maptileslayer.h ./src/editor/mapeditor.cc ./src/editor/mapeditor.h ./src/editor/mapobject.cc ./src/editor/mapobject.h ./src/editor/scenes/editormenu.cc ./src/editor/scenes/editormenu.h ./src/framework/animation.cc ./src/framework/animation.h ./src/framework/animationparser.cc ./src/framework/animationparser.h ./src/framework/animationset.cc ./src/framework/animationset.h ./src/framework/audiomanager.cc ./src/framework/audiomanager.h ./src/framework/drawable.h ./src/framework/engine.cc ./src/framework/engine.h ./src/framework/font.cc ./src/framework/font.h ./src/framework/frame.cc ./src/framework/frame.h ./src/framework/image.cc ./src/framework/image.h ./src/framework/inputmanager.cc ./src/framework/inputmanager.h ./src/framework/keys.h ./src/framework/layer.cc ./src/framework/layer.h ./src/framework/light.cc ./src/framework/light.h ./src/framework/modifier.cc ./src/framework/modifier.h ./src/framework/music.cc ./src/framework/music.h ./src/framework/observer.h ./src/framework/pathmanager.cc ./src/framework/pathmanager.h ./src/framework/sample.cc ./src/framework/sample.h ./src/framework/scene.cc ./src/framework/scene.h ./src/framework/sprite.cc ./src/framework/sprite.h ./src/framework/text.cc ./src/framework/text.h ./src/framework/textmanager.cc ./src/framework/textmanager.h ./src/framework/timeaccumulator.cc ./src/framework/timeaccumulator.h ./src/framework/timehandler.cc ./src/framework/timehandler.h ./src/framework/types.h ./src/framework/vector2D.cc ./src/framework/vector2D.h ./src/framework/videomanager.cc ./src/framework/videomanager.h ./src/game/config.h ./src/game/main.cc ./src/game/scenes/imagescene.cc ./src/game/scenes/imagescene.h ./src/game/scenes/loading.cc ./src/game/scenes/loading.h ./src/game/scenes/menu.cc ./src/game/scenes/menu.h ./src/game/scenes/menubuilder.cc ./src/game/scenes/menubuilder.h ./src/game/scenes/menuhandler.h ./src/game/scenes/page.cc ./src/game/scenes/page.h ./src/game/scenes/pagemanager.cc ./src/game/scenes/pagemanager.h ./src/game/scenes/pausescene.cc ./src/game/scenes/pausescene.h ./src/game/scenes/scrollingimagescene.cc ./src/game/scenes/scrollingimagescene.h ./src/game/scenes/world.cc ./src/game/scenes/world.h ./src/game/sprites/condition.h ./src/game/sprites/conditionbuilder.cc ./src/game/sprites/conditionbuilder.h ./src/game/sprites/creature.cc ./src/game/sprites/creature.h ./src/game/sprites/door.cc ./src/game/sprites/door.h ./src/game/sprites/explosion.cc ./src/game/sprites/explosion.h ./src/game/sprites/fireball.cc ./src/game/sprites/fireball.h ./src/game/sprites/floor.cc ./src/game/sprites/floor.h ./src/game/sprites/hero.cc ./src/game/sprites/hero.h ./src/game/sprites/item.cc ./src/game/sprites/item.h ./src/game/sprites/itembuilder.cc ./src/game/sprites/itembuilder.h ./src/game/sprites/itemevent.h ./src/game/sprites/lightningbolt.cc ./src/game/sprites/lightningbolt.h ./src/game/sprites/magicmissile.cc ./src/game/sprites/magicmissile.h ./src/game/sprites/mummy.cc ./src/game/sprites/mummy.h ./src/game/sprites/mummybuilder.cc ./src/game/sprites/mummybuilder.h ./src/game/sprites/mummyprojectile.cc ./src/game/sprites/mummyprojectile.h ./src/game/sprites/pharaoh.cc ./src/game/sprites/pharaoh.h ./src/game/sprites/projectile.cc ./src/game/sprites/projectile.h ./src/game/sprites/wall.cc ./src/game/sprites/wall.h ./src/game/sprites/weapons/herobaseweapon.cc ./src/game/sprites/weapons/herobaseweapon.h ./src/game/sprites/weapons/heroexplosionweapon.cc ./src/game/sprites/weapons/heroexplosionweapon.h ./src/game/sprites/weapons/herofireballweapon.cc ./src/game/sprites/weapons/herofireballweapon.h ./src/game/sprites/weapons/herolightningweapon.cc ./src/game/sprites/weapons/herolightningweapon.h ./src/game/sprites/weapons/mummyrangedweapon.cc ./src/game/sprites/weapons/mummyrangedweapon.h ./src/game/sprites/weapons/mummyweapon.cc ./src/game/sprites/weapons/mummyweapon.h ./src/game/sprites/weapons/pharaohrangedweapon.cc ./src/game/sprites/weapons/pharaohrangedweapon.h ./src/game/sprites/weapons/pharaohsummonweapon.cc ./src/game/sprites/weapons/pharaohsummonweapon.h ./src/game/sprites/weapons/weapon.h ./src/game/sprites/worldobject.cc ./src/game/sprites/worldobject.h ./src/game/utils/circleobject.cc ./src/game/utils/circleobject.h ./src/game/utils/collisionobject.h ./src/game/utils/constants.cc ./src/game/utils/constants.h ./src/game/utils/geometryprimitives.cc ./src/game/utils/geometryprimitives.h ./src/game/utils/hud.cc ./src/game/utils/hud.h ./src/game/utils/hudimagefactory.cc ./src/game/utils/hudimagefactory.h ./src/game/utils/imagefactory.cc ./src/game/utils/imagefactory.h ./src/game/utils/levelloader.cc ./src/game/utils/levelloader.h ./src/game/utils/levelmanager.cc ./src/game/utils/levelmanager.h ./src/game/utils/rectobject.cc ./src/game/utils/rectobject.h ./src/game/utils/settings.cc ./src/game/utils/settings.h ./src/game/utils/textloader.cc ./src/game/utils/textloader.h ./src/game/utils/tile.cc ./src/game/utils/tile.h ./src/game/utils/visionstrategy.cc ./src/game/utils/visionstrategy.h )

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

