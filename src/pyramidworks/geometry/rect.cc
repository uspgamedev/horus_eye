#include "rect.h"
#include "circle.h"
#include <cmath>

namespace pyramidworks {
namespace geometry {


using namespace ugdk;

bool Rect::Intersects (const Rect *rect) const {
    Vector2D    otherpos = rect->position(),
                thispos  = this->position();
    if((thispos.x > otherpos.x + rect->width()) ||
       (thispos.y > otherpos.y + rect->height()) ||
       (thispos.x + this->width() < otherpos.x) ||
       (thispos.y + this->height() < otherpos.y))
        return false;
    else
        return true;
}

bool Rect::Intersects (const Circle *circle) const {

    Vector2D distance = (circle->position()) - (this->position());

	if(fabs(distance.x) + fabs(distance.y) > circle->radius() + 2*half_width_ + 2*half_height_)
		return false;

    if (fabs(distance.y) <= half_height_ &&
        fabs(distance.x) <= circle->radius() + half_width_)
        return true;
    if (fabs(distance.x) <= half_width_ &&
        fabs(distance.y) <= circle->radius() + half_height_)
        return true;

    Vector2D circ_pos = circle->position() + circle->offset(),
             to_vert(half_width_, half_height_);

    Vector2D circminuspos = circ_pos - (offset() + this->position());
    distance = circminuspos - to_vert;
    if (distance.x > 0 && distance.y > 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    distance = circminuspos + to_vert;
    if (distance.x > 0 && distance.y < 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    to_vert.x *= -1;
    distance = circminuspos + to_vert;
    if (distance.x < 0 && distance.y > 0 &&
        distance.length() <= circle->radius())
        return true;

    to_vert.y *= -1;
    distance = circminuspos + to_vert;
    if (distance.x < 0 && distance.y < 0 &&
        distance.length() <= circle->radius())
        return true;

    return false;

}


bool Rect::Intersects (const GeometricObject *coll_obj) const {
    return coll_obj->Intersects(this);
}



} // namespace geometry
} // namespace pyramidworks
