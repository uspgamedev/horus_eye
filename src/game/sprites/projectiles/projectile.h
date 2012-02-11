#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "game/sprites/timedworldobject.h"
#include <ugdk/math/vector2D.h>

namespace sprite {

class Projectile : public TimedWorldObject {
  private: typedef TimedWorldObject super;
  public:
    Projectile(double speed, int duration, ugdk::Vector2D &dir);
    void Move(double delta_t);
    virtual void Update(double delta_t);

    void set_collision_object(CollisionObject* col) { collision_object_ = col; }

  protected:
    double speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
