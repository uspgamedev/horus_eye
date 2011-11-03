
#include "circle.h"
#include "rect.h"

namespace pyramidworks {
namespace geometry {

using namespace ugdk;

bool Circle::Intersects (const Rect *rect) const {
    return rect->Intersects(this);
}

bool Circle::Intersects (const Circle *circle) const {
    Vector2D distance = circle->position() - this->position();
    return distance.length() <= circle->radius_ + this->radius_;
}

bool Circle::Intersects (const GeometricShape *coll_obj) const {
    return coll_obj->Intersects(this);
}

} // namespace geometry
} // namespace pyramidworks
