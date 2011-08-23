#ifndef HORUSEYE_GAME_SPRITES_FIREBALL_H_
#define HORUSEYE_GAME_SPRITES_FIREBALL_H_

#include "worldobject.h"
#include "explosion.h"
#include "magicmissile.h"
#include "ugdk/vector2D.h"

namespace sprite {

class Mummy;

class Fireball : public Projectile {
  public:
    Fireball(ugdk::Vector2D &dir, Explosion *explosion);
    virtual ~Fireball();

    virtual void CollidesWith(Mummy * obj) { Explode(); }
    virtual void HandleCollision(WorldObject *);
    virtual void Update(float);

  protected:
    virtual void Explode();
    Explosion *explosion_;

    void InitializeAnimations();
    static ugdk::AnimationSet *ANIMATIONS;
    static ugdk::uint32 attacking_animations_[8];
};

}

#endif 
