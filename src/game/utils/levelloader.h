//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/levelloader.h
// Definicao da classe LevelLoader.
//

#ifndef ORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define ORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <string>
#include "../scenes/world.h"

namespace utils {

class LevelLoader {
  public:
    LevelLoader(scene::World * world) : world_(world) {}
    virtual ~LevelLoader() {}

    void Load(std::string);

  protected:

    scene::World * world_;

};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
