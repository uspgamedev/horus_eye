#ifndef PYRAMIDWORKS_GEOMETRY_RECT_H_
#define PYRAMIDWORKS_GEOMETRY_RECT_H_

#include "geometricshape.h"

namespace pyramidworks {
namespace geometry {

class Circle;

class Rect : public GeometricShape {
  public:
    Rect(float width, float height)
      : half_width_(width/2), half_height_(height/2) {}
    virtual ~Rect() {}

    float width() const { return 2*half_width_; }
    float height() const { return 2*half_height_; }

    bool Intersects (const Rect *) const;
    bool Intersects (const Circle *) const;
    bool Intersects (const GeometricShape *) const;

  private:
    float half_width_, half_height_;
};

} // namespace geometry
} // namespace pyramidworks


#endif /* PYRAMIDWORKS_GEOMETRY_RECT_H_ */
