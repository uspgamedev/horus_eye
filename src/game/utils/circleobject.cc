#include "circleobject.h"
#include "rectobject.h"

namespace utils {

using namespace framework;

bool CircleObject::Intersects (const RectObject *rect) const {

    return rect->Intersects(this);

}

bool CircleObject::Intersects (const CircleObject *circle) const {

    Vector2D distance = circle->position() - position();
    return distance.length() <= circle->radius_ + this->radius_;

}

bool CircleObject::Intersects (const CollisionObject *coll_obj) const {

    return coll_obj->Intersects(this);

}

} // namespace utils

