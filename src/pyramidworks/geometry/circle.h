#ifndef PYRAMIDWORKS_GEOMETRY_CIRCLE_H_
#define PYRAMIDWORKS_GEOMETRY_CIRCLE_H_

#include <pyramidworks/geometry/geometricshape.h>

namespace pyramidworks {
namespace geometry {

/// A circle, one type of GeometricShape.
class Circle : public GeometricShape {
  public:
    /** @param radius The circle radius. */
    Circle(float radius) : radius_(radius) {}

    /// Getter for the radius.
    /** @return A float. */
    float radius() const { return radius_; }

    /// Setter the radius.
    /** @param radius The new radius. */
    void set_radius(float radius) { radius_ = radius; }

    bool Intersects (const GeometricShape *) const;
    bool Intersects (const Rect *) const;
    bool Intersects (const Circle *) const;

  private:
    float radius_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_CIRCLE_H_ */
