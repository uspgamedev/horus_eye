#ifndef HORUSEYE_GAME_SPRITES_LIGHTNINGBOLT_H_
#define HORUSEYE_GAME_SPRITES_LIGHTNINGBOLT_H_
#include <set>

#include "projectile.h"
#include "../../framework/vector2D.h"

namespace framework {
class TimeAccumulator;
class AnimationSet;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class Creature;

class LightningBolt : public Projectile {
  public:
    LightningBolt(framework::Vector2D &);
    ~LightningBolt();

    virtual void CollidesWith(Mummy * obj);
    virtual void HandleCollision(WorldObject *);

  protected:
    void InitializeAnimations();
    static framework::AnimationSet *ANIMATIONS;
    static framework::uint32 attacking_animations_[8];
};

}

#endif /* HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_ */