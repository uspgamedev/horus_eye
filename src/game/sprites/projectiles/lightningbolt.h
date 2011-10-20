#ifndef HORUSEYE_GAME_SPRITES_LIGHTNINGBOLT_H_
#define HORUSEYE_GAME_SPRITES_LIGHTNINGBOLT_H_

#include "game/sprites/projectiles/projectile.h"
#include <ugdk/math/vector2D.h>

namespace ugdk {
class TimeAccumulator;
class AnimationSet;
}

namespace sprite {

class LightningBolt : public Projectile {
  public:
    LightningBolt(ugdk::Vector2D &);
	~LightningBolt() {}

  protected:
    void InitializeAnimations();
    static ugdk::AnimationSet *ANIMATIONS;
    static ugdk::uint32 attacking_animations_[8];
};

}

#endif /* HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_ */
