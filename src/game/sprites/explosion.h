#ifndef HORUSEYE_GAME_SPRITES_EXPLOSION_H_
#define HORUSEYE_GAME_SPRITES_EXPLOSION_H_

#include "game/sprites/worldobject.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/action/observer.h>

namespace ugdk {
class TimeAccumulator;
class AnimationSet;
}

namespace sprite {

class Explosion : public WorldObject, ugdk::Observer {

  public:
    Explosion(ugdk::Image *img, ugdk::uint32 animation, float radius, float damage);
    ~Explosion();

    static void InitializeAnimations();
    static void ReleaseAnimations();

    void RadiusUpdate(float delta_t);
    void Update(float delta_t);
    virtual void Tick();

    int damage() { return damage_; }

    const static int    HERO_FIREBALL_WEAPON,
                        HERO_EXPLOSION_WEAPON;

  protected:

    static ugdk::AnimationSet  *ANIMATIONS;
    static ugdk::uint32        WEAPON_ANIMATIONS[2];

	COLLISION_BEGIN
		COLLISION_ADD		(Explosion, Damage)
	COLLISION_END

  private:
	int damage_;
    float radius_;
    float expansion_speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_EXPLOSION_H_ */
