//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/collisionobjectr.h
// Definicao da interface CollisionObject.
//

#ifndef HORUSEYE_GAME_UTILS_COLLISIONOBJECT_H_
#define HORUSEYE_GAME_UTILS_COLLISIONOBJECT_H_

#include "../../framework/vector2D.h"

namespace utils {

class CircleObject;
class RectObject;

class CollisionObject {
  public:
    virtual ~CollisionObject() {}
    virtual bool Intersects (const RectObject *)  const { return false; }
    virtual bool Intersects (const CircleObject *) const { return false; }
    virtual bool Intersects (const CollisionObject *) const { return false; }
    virtual framework::Vector2D position () const { return position_; }
    virtual void set_position (const framework::Vector2D &position) {
        position_ = position;
    }
  protected:
    CollisionObject() {}
  private:
    framework::Vector2D position_;
};

} // namespace utils

#endif /* COLLISIONOBJECT_H_ */