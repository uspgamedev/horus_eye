#ifndef HORUSEYE_GAME_SPRITES_CARRIER_H_
#define HORUSEYE_GAME_SPRITES_CARRIER_H_

#include "game/sprites/projectiles/projectile.h"
#include <ugdk/math/vector2D.h>

namespace sprite {

class Explosion;
class Carrier : public Projectile {
  public:
    Carrier(float speed, int duration, ugdk::Vector2D &dir, WorldObject *explosion) :
        Projectile(0.0f, speed, duration, dir), explosion_(explosion) {}

  protected:
    virtual void Explode();
    WorldObject *explosion_;
};

}

#endif // HORUSEYE_GAME_SPRITES_CARRIER_H_
