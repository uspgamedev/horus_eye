//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/levelloader.h
// Definicao da classe LevelLoader.
//

#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <string>
#include "../scenes/world.h"

namespace utils {

class LevelLoader {
  public:
    char **matrix_;
    int width_, height_;
    LevelLoader(scene::World * world) : world_(world) {}
    virtual ~LevelLoader() {}

    void Load(std::string);
    void LoadMatrix(std::string);

  protected:

    scene::World * world_;

};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
