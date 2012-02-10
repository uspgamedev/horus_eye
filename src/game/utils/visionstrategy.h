#ifndef HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
#define HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_

#include <ugdk/math/Vector2D.h>
#include <queue>

using ugdk::Vector2D;

namespace utils {

class VisionStrategy {
  public:
    std::queue<Vector2D> Calculate(Vector2D position);
    bool IsVisible(Vector2D position1, Vector2D position2 = Vector2D(-1,-1));
    bool IsLightVisible(Vector2D position1, Vector2D position2 = Vector2D(-1,-1));
};

}

#endif // HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
