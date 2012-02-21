
#include "circle.h"
#include "rect.h"
#include <ugdk/util/intervalkdtree.h>

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

ugdk::ikdtree::Box<2> Circle::GetBoundingBox () const {
    Vector2D thispos = this->position();
    Vector2D thisposmin(thispos.x - radius_,
                        thispos.y - radius_);
    Vector2D thisposmax(thispos.x + radius_,
                        thispos.y + radius_);
    return ugdk::ikdtree::Box<2>(thisposmin.val, thisposmax.val);
}
} // namespace geometry
} // namespace pyramidworks
