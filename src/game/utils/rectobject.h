#ifndef HORUSEYE_GAME_UTILS_RECTOBJECT_H_
#define HORUSEYE_GAME_UTILS_RECTOBJECT_H_

#include "collisionobject.h"

namespace utils {

class CircleObject;

class RectObject : public CollisionObject {
  public:
    RectObject(float width, float height)
      : half_width_(width/2), half_height_(height/2) {}
    virtual ~RectObject() {}

    float width() const { return 2*half_width_; }
    float height() const { return 2*half_height_; }

    bool Intersects (const RectObject *) const;
    bool Intersects (const CircleObject *) const;
    bool Intersects (const CollisionObject *) const;

  private:
    float half_width_, half_height_;
};

} // namespace utils

#endif /* RECTOBJECT_H_ */
