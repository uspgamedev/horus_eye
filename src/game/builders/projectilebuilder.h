#ifndef HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace utils {
class ImageFactory;
class IsometricAnimationSet;
} // namespace utils

namespace builder {

class ProjectileBuilder {
  public:
    ProjectileBuilder(utils::ImageFactory *factory) : factory_(factory) { InitializeAnimations(); }
    ~ProjectileBuilder() {}

    sprite::WorldObject* MagicMissile(const ugdk::Vector2D &dir);
    sprite::WorldObject* MagicBall(const ugdk::Vector2D &dir);
    sprite::WorldObject* MummyProjectile(const ugdk::Vector2D &dir, int damage = 1);
    sprite::WorldObject* LightningBolt(const ugdk::Vector2D &dir);
    sprite::WorldObject* Fireball(const ugdk::Vector2D &dir);

  private:
    utils::ImageFactory *factory_;
    static utils::IsometricAnimationSet *fireball_animation_, *lightning_animation_;
    void InitializeAnimations();
};

} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_ */
