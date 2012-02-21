#ifndef PYRAMIDWORKS_GEOMETRY_RECT_H_
#define PYRAMIDWORKS_GEOMETRY_RECT_H_

#include <pyramidworks/geometry/geometricshape.h>

namespace pyramidworks {
namespace geometry {

/// \class Rect rect.h "pyramidworks/geometry/rect.h"
/// A rect, one type of GeometricShape.
class Rect : public GeometricShape {
  public:
    /** @param width  The rect width. 
      * @param height The rect height.
      * @param offset The shape offset. Defaults to Vector2D(0, 0) */
    Rect(double width, double height, ugdk::Vector2D offset = ugdk::Vector2D())
      : GeometricShape(offset), half_width_(width/2), half_height_(height/2) {}
    
    /// Returns the rect width.
    /** @return A double. */
    double width() const { return 2*half_width_; }

    /// Returns the rect height.
    /** @return A double. */
    double height() const { return 2*half_height_; }

    bool Intersects (const GeometricShape *) const;
    bool Intersects (const Rect *) const;
    bool Intersects (const Circle *) const;
    ugdk::ikdtree::Box<2> GetBoundingBox () const;

  private:
    double half_width_, half_height_;
};

} // namespace geometry
} // namespace pyramidworks


#endif /* PYRAMIDWORKS_GEOMETRY_RECT_H_ */
