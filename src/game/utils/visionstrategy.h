#ifndef HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
#define HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_

#include <ugdk/math/vector2D.h>
#include <queue>
#include "game/sprites.h"

using ugdk::math::Vector2D;

namespace utils {

class VisionStrategy {
  public:
    std::queue<Vector2D> Calculate(sprite::WObjRawPtr who);

    bool IsVisible(const ugdk::math::Vector2D& from, const ugdk::math::Vector2D& target);
    bool IsVisible(sprite::WObjRawPtr from, const ugdk::math::Vector2D& target);
    bool IsVisible(sprite::WObjRawPtr from);

    bool IsLightVisible(Vector2D position1, Vector2D position2 = Vector2D(-1,-1));
};

}

#endif // HORUSEYE_GAME_UTILS_VISIONSTRATEGY_H_
