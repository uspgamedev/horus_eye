//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/circleobject.h
// Definicao da classe CircleObject.
//

#ifndef HORUSEYE_GAME_UTILS_CIRCLEOBJECT_H_
#define HORUSEYE_GAME_UTILS_CIRCLEOBJECT_H_

#include "collisionobject.h"

namespace utils {

class RectObject;

class CircleObject : public CollisionObject {
  public:
    CircleObject(float radius) : radius_(radius) {}
    virtual ~CircleObject() {}

    float radius() const { return radius_; }

    bool Intersects (const RectObject *) const;
    bool Intersects (const CircleObject *) const;
    bool Intersects (const CollisionObject *) const;

  private:
    float radius_;
};

} // namespace utils

#endif /* CIRCLEOBJECT_H_ */
