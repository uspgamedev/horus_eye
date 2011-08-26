#ifndef HORUSEYE_GAME_UTILS_COLLISIONOBJECT_H_
#define HORUSEYE_GAME_UTILS_COLLISIONOBJECT_H_

#include "ugdk/vector2D.h"

namespace utils {

class CircleObject;
class RectObject;

class CollisionObject {
  public:
    virtual ~CollisionObject() {}
    virtual bool Intersects (const RectObject *)  const { return false; }
    virtual bool Intersects (const CircleObject *) const { return false; }
    virtual bool Intersects (const CollisionObject *) const { return false; }
    virtual ugdk::Vector2D position () const { return position_; }
    virtual void set_position (const ugdk::Vector2D &position) {
        position_ = position;
    }
  protected:
    CollisionObject() {}
  private:
    ugdk::Vector2D position_;
};

} // namespace utils

#endif /* COLLISIONOBJECT_H_ */
