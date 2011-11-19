#ifndef PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_
#define PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_

#include <ugdk/math/vector2D.h>

namespace pyramidworks {
namespace geometry {

class Circle;
class Rect;

/// A geometric shape.
class GeometricShape {
  public:
    virtual ~GeometricShape() {}

    /// Checks if this shape intersects another shape.
    /** @param obj The shape to compare with.
        @return True if there's an intersection. */
    virtual bool Intersects (const GeometricShape *obj) const = 0;

    /// Checks if there's an intersection with a Circle.
    /** @param obj The shape to compare with.
        @return True if there's an intersection. */
    virtual bool Intersects (const Circle *obj) const = 0;

    /// Checks if there's an intersection with a Rect.
    /** @param obj The shape to compare with.
        @return True if there's an intersection. */
    virtual bool Intersects (const Rect *obj) const = 0;

    /// Getter for the offset.
    /** @return A reference to the current offset. */
    const ugdk::Vector2D& offset() const { return offset_; }

    /// Changes the shape's offset.
    /** @param offset The new offset. */
    void set_offset (const ugdk::Vector2D &offset) {
        offset_ = offset;
    }

    /// Sets a pointer to a position.
    /** @param pos A pointer to a ugdk::Vector2D. May be NULL.
    */
    void set_position(const ugdk::Vector2D *pos) { position_ = pos; }

    /// Gets the shape's position.
    /** Adds the offset to the vector pointed by position_, if it's not NULL.
        @return A Vector2D representing the position. */
    ugdk::Vector2D position() const { 
        return (position_ != 0) ? offset_ + *position_ : offset_;
    }

  protected:
    /** @param offset An offset to the position. Defaults to (0,0) */
    GeometricShape(const ugdk::Vector2D offset = ugdk::Vector2D()) 
        : position_(0), offset_(offset) {}

  private:
    const ugdk::Vector2D *position_;
    ugdk::Vector2D offset_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_ */
