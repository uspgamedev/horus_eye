#ifndef HORUSEYE_GAME_SPRITES_CARRIER_H_
#define HORUSEYE_GAME_SPRITES_CARRIER_H_

#include <list>
#include <ugdk/math/vector2D.h>

#include "game/sprites/projectiles/projectile.h"

namespace sprite {

class Explosion;
class Carrier : public Projectile {
  public:
    Carrier(float speed, int duration, ugdk::Vector2D &dir, std::list<WorldObject*> &list) 
        :   Projectile(0.0f, speed, duration, dir), 
            drop_list_(list) 
            {}

    Carrier(float speed, int duration, ugdk::Vector2D &dir, WorldObject *drop) 
        :   Projectile(0.0f, speed, duration, dir)
    {
        drop_list_.push_back(drop);
    }

    virtual void Die();
  protected:
    std::list<WorldObject*> drop_list_;
};

}

#endif // HORUSEYE_GAME_SPRITES_CARRIER_H_
