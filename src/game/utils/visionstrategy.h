
//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/visionstrategy.h
// Definicao da classe LevelLoader.
//

#ifndef HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
#define HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_

#include "../scenes/world.h"
#include<queue>

namespace utils {

class VisionStrategy {
  public:
    std::queue<Vector2D> Calculate(Vector2D position);
    bool IsVisible(Vector2D position1, Vector2D position2 = Vector2D(-1,-1));
    bool IsLightVisible(Vector2D position1, Vector2D position2 = Vector2D(-1,-1));
};

}

#endif // HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
