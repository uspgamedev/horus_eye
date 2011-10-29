#ifndef HORUSEYE_GAME_SPRITES_FIREBALL_H_
#define HORUSEYE_GAME_SPRITES_FIREBALL_H_

#include "game/sprites/projectiles/projectile.h"
#include <ugdk/math/vector2D.h>

namespace sprite {

class Explosion;
class Fireball : public Projectile {
  public:
    Fireball(ugdk::Vector2D &dir, Explosion *explosion);

  protected:
    virtual void Explode();
    Explosion *explosion_;

    void InitializeAnimations();
    static ugdk::AnimationSet *ANIMATIONS;
    static ugdk::uint32 attacking_animations_[8];
};

}

#endif 
