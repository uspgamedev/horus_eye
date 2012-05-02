#ifndef HORUSEYE_GAME_SPRITES_EXPLOSION_H_
#define HORUSEYE_GAME_SPRITES_EXPLOSION_H_

#include "game/sprites/worldobject.h"
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/observer.h>
#include <ugdk/base/types.h>
#include <pyramidworks/geometry.h>

namespace sprite {

class Explosion : public WorldObject, ugdk::action::Observer {
  typedef WorldObject super;
  public:
    Explosion(ugdk::graphic::FlexibleSpritesheet *img, ugdk::uint32 animation, double radius, double damage);
    ~Explosion();

    static void InitializeAnimations();
    static void ReleaseAnimations();

    void RadiusUpdate(double delta_t);
    void Update(double delta_t);
    virtual void Tick();
    void set_world_position(const ugdk::Vector2D& pos);

    int damage() { return damage_; }

    const static int    HERO_FIREBALL_WEAPON,
                        HERO_EXPLOSION_WEAPON;

  protected:

    static ugdk::action::AnimationSet  *ANIMATIONS;
    static ugdk::uint32        WEAPON_ANIMATIONS[2];

  private:
	int damage_;
    double radius_;
    double expansion_speed_;
    ugdk::Vector2D direction_;
    pyramidworks::geometry::Circle* bound_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_EXPLOSION_H_ */
