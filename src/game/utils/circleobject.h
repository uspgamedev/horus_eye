#ifndef HORUSEYE_GAME_UTILS_CIRCLEOBJECT_H_
#define HORUSEYE_GAME_UTILS_CIRCLEOBJECT_H_

#include "game/utils/collisionobject.h"

namespace utils {

class RectObject;

class CircleObject : public CollisionObject {
  public:
    CircleObject(float radius) : radius_(radius) {}
    virtual ~CircleObject() {}

    float radius() const { return radius_; }
    void set_radius(float radius) { radius_ = radius; }

    bool Intersects (const RectObject *) const;
    bool Intersects (const CircleObject *) const;
    bool Intersects (const CollisionObject *) const;

  private:
    float radius_;
};

} // namespace utils

#endif /* CIRCLEOBJECT_H_ */
