#include "game/components/direction.h"

#include "game/core/coordinates.h"

namespace component {

Direction Direction::FromScreenVector(const ugdk::Vector2D& versor) {
    ugdk::Vector2D versorN = versor.Normalize();
    Direction d;
    if(versorN.x >  0.33) d.direction_ |= RIGHT;
    if(versorN.x < -0.33) d.direction_ |=  LEFT;
    if(versorN.y >  0.33) d.direction_ |=  DOWN;
    if(versorN.y < -0.33) d.direction_ |=    UP;
    return d;
}

Direction Direction::FromWorldVector(const ugdk::Vector2D& versor) {
    return FromScreenVector(core::FromWorldLinearCoordinates(versor));
}

}  // namespace component
