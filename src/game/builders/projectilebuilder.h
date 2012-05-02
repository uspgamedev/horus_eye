#ifndef HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include "game/sprites/projectiles/projectile.h"

namespace utils {
class ImageFactory;
} // namespace utils

namespace builder {

class ProjectileBuilder {
  public:
    ProjectileBuilder(utils::ImageFactory *factory) : factory_(factory) { InitializeAnimations(); }
    ~ProjectileBuilder() {}

    sprite::Projectile* MagicMissile(ugdk::Vector2D &dir);
    sprite::Projectile* MummyProjectile(ugdk::Vector2D &dir, int damage = 1);
    sprite::Projectile* LightningBolt(ugdk::Vector2D &dir);
    sprite::Projectile* Fireball(ugdk::Vector2D &dir);

  private:
    utils::ImageFactory *factory_;
    static ugdk::action::AnimationSet *fireball_animation_, *lightning_animation_;
    static ugdk::uint32 fireball_animation_map_[8], lightning_animation_map_[8];

    void InitializeAnimations();
};

} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_ */
