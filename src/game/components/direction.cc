#include "game/components/direction.h"

#include <cmath>

#include "game/core/coordinates.h"

namespace component {

Direction Direction::FromScreenVector(const ugdk::math::Vector2D& versor) {
    ugdk::math::Vector2D versorN = versor.Normalize();
    Direction d;
    if(versorN.x >  0.33) d.direction_ |= RIGHT;
    if(versorN.x < -0.33) d.direction_ |=  LEFT;
    if(versorN.y >  0.33) d.direction_ |=  DOWN;
    if(versorN.y < -0.33) d.direction_ |=    UP;
    return d;
}

Direction Direction::FromWorldVector(const ugdk::math::Vector2D& versor) {
	static double PI_8 = std::atan(1.0) / 2.0;
	double angle = versor.Angle();

	if(       angle < -7*PI_8) {
		return Direction(DOWN | LEFT);
	} else if(angle < -5*PI_8) {
		return Direction(DOWN);
	} else if(angle < -3*PI_8) {
		return Direction(DOWN | RIGHT);
	} else if(angle < -1*PI_8) {
		return Direction(RIGHT);
	} else if(angle <  1*PI_8) {
		return Direction(UP | RIGHT);
	} else if(angle <  3*PI_8) {
		return Direction(UP);
	} else if(angle <  5*PI_8) {
		return Direction(UP | LEFT);
	} else if(angle <  7*PI_8) {
		return Direction(LEFT);
	} else {
		return Direction(DOWN | LEFT);
	}
}

}  // namespace component
