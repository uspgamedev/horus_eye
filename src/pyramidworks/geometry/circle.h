#ifndef PYRAMIDWORKS_GEOMETRY_CIRCLE_H_
#define PYRAMIDWORKS_GEOMETRY_CIRCLE_H_

#include "geometricobject.h"

namespace pyramidworks {
namespace geometry {

class Rect;

class Circle : public GeometricObject {
  public:
    Circle(float radius) : radius_(radius) {}
    virtual ~Circle() {}

    float radius() const { return radius_; }
    void set_radius(float radius) { radius_ = radius; }

    bool Intersects (const Rect *) const;
    bool Intersects (const Circle *) const;
    bool Intersects (const GeometricObject *) const;

  private:
    float radius_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_CIRCLE_H_ */
