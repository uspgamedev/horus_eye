#include "rectobject.h"
#include "circleobject.h"
#include <cmath>

namespace utils {

using namespace ugdk;

bool RectObject::Intersects (const RectObject *rect) const {
    return false;
}

bool RectObject::Intersects (const CircleObject *circle) const {

    Vector2D distance = circle->position() - position();

	if(fabs(distance.x) + fabs(distance.y) > circle->radius() + 2*half_width_ + 2*half_height_)
		return false;

    if (fabs(distance.y) <= half_height_ &&
        fabs(distance.x) <= circle->radius() + half_width_)
        return true;
    if (fabs(distance.x) <= half_width_ &&
        fabs(distance.y) <= circle->radius() + half_height_)
        return true;

    Vector2D circ_pos = circle->position(),
             to_vert(half_width_, half_height_);

    distance = circ_pos - (position() + to_vert);
    if (distance.x > 0 && distance.y > 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    distance = circ_pos - (position() + to_vert);
    if (distance.x > 0 && distance.y < 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    to_vert.x *= -1;
    distance = circ_pos - (position() + to_vert);
    if (distance.x < 0 && distance.y > 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    distance = circ_pos - (position() + to_vert);
    if (distance.x < 0 && distance.y < 0 &&
        distance.length() <= circle->radius())
        return true;

    return false;

}


bool RectObject::Intersects (const CollisionObject *coll_obj) const {
    return coll_obj->Intersects(this);
}


} // namespace utils
