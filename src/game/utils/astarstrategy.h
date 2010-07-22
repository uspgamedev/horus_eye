
//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/astarstrategy.h
// Definicao da classe AStarStrategy.
//

#ifndef HORUSEYE_GAME_UTILS_ASTARSTRATEGY_H_
#define HORUSEYE_GAME_UTILS_ASTARSTRATEGY_H_

#include <queue>
#include "../../framework/vector2D.h"
using namespace framework;
using namespace std;
namespace utils {

class AStarStrategy {
  public:
    queue<Vector2D> Calculate(Vector2D position);
};

}

#endif // HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
