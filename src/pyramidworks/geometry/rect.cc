#include "rect.h"
#include "circle.h"
#include <cmath>

#define SQRT_TWO 1.4142136

namespace pyramidworks {
namespace geometry {


using namespace ugdk;

bool Rect::Intersects (const Rect *rect) const {
    Vector2D    otherpos = rect->position(),
                thispos  = this->position();

    if( (thispos.x - half_width_  > otherpos.x + rect->half_width_ ) ||
        (thispos.x + half_width_  < otherpos.x - rect->half_width_ ) ||
        (thispos.y - half_height_ > otherpos.y + rect->half_height_) ||
        (thispos.y + half_height_ < otherpos.y - rect->half_height_))
        return false;
    else
        return true;
}

bool Rect::Intersects (const Circle *circle) const {

    Vector2D circ_pos = circle->position(),
             rect_pos = this->position(),
             distance = circ_pos - rect_pos,
             abs_dist = Vector2D(fabs(distance.x),fabs(distance.y));
    
    double radius = circle->radius();

    //TODO:TEST this, and test if this helps performance. Probably not.
    /*
    // Simple norm 1 check. Superseeded by the circunscript rectangle comparing.
    if( SQRT_TWO*radius + half_width_ + half_height_ < (abs_dist.x + abs_dist.y) )
        return false;
    */

    // These are needed for the non-corner collisions.
    double rect_left  = rect_pos.x - half_width_,
          rect_right = rect_pos.x + half_width_,
          rect_down  = rect_pos.y - half_height_,
          rect_up    = rect_pos.y + half_height_;

    // Comparing the circunscript rect around the circle with the rect.
    // This takes care of most collision checks, including all non-corner ones.
    if(      (radius + half_width_ ) < abs_dist.x )
        return false;
    else if( circ_pos.y >= rect_down && circ_pos.y <= rect_up ) // non-corner x collision check.
        return true;
    else if( (radius + half_height_) < abs_dist.y )
        return false;
    else if( circ_pos.x >= rect_left && circ_pos.x <= rect_right ) // non-corner y collision check.
        return true;

    // From now on, we'll treat only corner collisions.

    // First, find the offending corner.
    Vector2D corner = Vector2D(rect_left,rect_down);
    if( circ_pos.x > rect_right ) corner.x = rect_right;
    if( circ_pos.y > rect_up    ) corner.y = rect_up;

    // Now find the important distance.
    Vector2D circ_to_corner = corner - circ_pos;
    double le_important_dist_sqr = circ_to_corner.LengthSquared();

    // Now, check if the circle is too far away from the corner.
    if( radius*radius < le_important_dist_sqr )
        return false;

    // We've checked all possibilities, and nothing, so it is colliding.
    return true;

    /*
    if (fabs(distance.y) <= half_height_ &&
        fabs(distance.x) <= circle->radius() + half_width_)
        return true;
    if (fabs(distance.x) <= half_width_ &&
        fabs(distance.y) <= circle->radius() + half_height_)
        return true;

    Vector2D to_vert(half_width_, half_height_);

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
    */
}


bool Rect::Intersects (const GeometricShape *coll_obj) const {
    return coll_obj->Intersects(this);
}



} // namespace geometry
} // namespace pyramidworks
