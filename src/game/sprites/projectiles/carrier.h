#ifndef HORUSEYE_GAME_SPRITES_CARRIER_H_
#define HORUSEYE_GAME_SPRITES_CARRIER_H_

#include <list>
#include <ugdk/math/vector2D.h>

#include "game/sprites/projectiles/projectile.h"

namespace sprite {

class Carrier : public Projectile {
  private: typedef Projectile super;
  public:
    Carrier(double speed, int duration, ugdk::Vector2D &dir, std::list<WorldObject*> &list) 
        :   Projectile(speed, duration, dir), 
            drop_list_(list) 
            {}

    Carrier(double speed, int duration, ugdk::Vector2D &dir, WorldObject *drop) 
        :   Projectile(speed, duration, dir) {
        drop_list_.push_back(drop);
    }

    virtual void Die();
  protected:
    std::list<WorldObject*> drop_list_;
};

}

#endif // HORUSEYE_GAME_SPRITES_CARRIER_H_
