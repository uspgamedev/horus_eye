#ifndef PYRAMIDWORKS_GEOMETRY_CIRCLE_H_
#define PYRAMIDWORKS_GEOMETRY_CIRCLE_H_

#include <pyramidworks/geometry/geometricshape.h>

namespace pyramidworks {
namespace geometry {

/// \class Circle circle.h "pyramidworks/geometry/circle.h"
/// A circle, one type of GeometricShape.
class Circle : public GeometricShape {
  public:
    /** @param radius The circle radius.
      * @param offset The shape offset. Defaults to Vector2D(0, 0) */
    Circle(double radius, ugdk::Vector2D offset = ugdk::Vector2D()) 
        : GeometricShape(offset), radius_(radius) {}

    /// Getter for the radius.
    /** @return A double. */
    double radius() const { return radius_; }

    /// Setter the radius.
    /** @param radius The new radius. */
    void set_radius(double radius) { radius_ = radius; }

    bool Intersects (const GeometricShape *) const;
    bool Intersects (const Rect *) const;
    bool Intersects (const Circle *) const;

  private:
    double radius_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_CIRCLE_H_ */
