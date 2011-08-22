#ifndef HORUSEYE_GAME_SPRITES_EXPLOSION_H_
#define HORUSEYE_GAME_SPRITES_EXPLOSION_H_

#include "worldobject.h"
#include "ugdk/vector2D.h"
#include "ugdk/observer.h"

namespace ugdk {
class TimeAccumulator;
class AnimationSet;
}

namespace sprite {

class Mummy;
class Explosion : public WorldObject, ugdk::Observer {

  public:
    Explosion(ugdk::Image *img, ugdk::uint32 animation, float radius, float damage);
    ~Explosion();

    static void InitializeAnimations();
    static void ReleaseAnimations();

    void RadiusUpdate(float delta_t);
    void Update(float delta_t);
    virtual void Tick();

    void CollidesWith(Mummy *);
    virtual void HandleCollision(WorldObject *);
    int damage() { return damage_; }

    const static int    HERO_FIREBALL_WEAPON,
                        HERO_EXPLOSION_WEAPON;

  protected:

    static ugdk::AnimationSet  *ANIMATIONS;
    static ugdk::uint32        WEAPON_ANIMATIONS[2];


  private:
	int damage_;
    float radius_;
    float expansion_speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_EXPLOSION_H_ */
