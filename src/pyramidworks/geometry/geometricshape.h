#ifndef PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_
#define PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_

#include <ugdk/math/vector2D.h>

namespace pyramidworks {
namespace geometry {

class Circle;
class Rect;

class GeometricShape {
  public:
    virtual ~GeometricShape() {}

    virtual bool Intersects (const Circle *) const { return false; }
    virtual bool Intersects (const Rect *) const { return false; }
    virtual bool Intersects (const GeometricShape *obj) const { return false; }

    virtual ugdk::Vector2D offset() const { return offset_; }
    virtual void set_offset (const ugdk::Vector2D &offset) {
        offset_ = offset;
    }

    void set_position(const ugdk::Vector2D *pos) { position_ = pos; }
    ugdk::Vector2D position() const { 
        return (position_ != 0) ? offset_ + *position_ : offset_;
    }

  protected:
    GeometricShape(ugdk::Vector2D offset = ugdk::Vector2D()) 
        : position_(0), offset_(offset) {}


  private:
    const ugdk::Vector2D *position_;
    ugdk::Vector2D offset_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_ */
