#ifndef HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/projectiles/projectile.h"

namespace ugdk {
class Image;
}

namespace utils {
class ImageFactory;
}

namespace builder {

class ProjectileBuilder {
  public:
    ProjectileBuilder(utils::ImageFactory *factory) : factory_(factory) {}
    ~ProjectileBuilder() {}

    sprite::Projectile* MagicMissile(ugdk::Vector2D &dir);
    sprite::Projectile* MummyProjectile(ugdk::Vector2D &dir, int damage = 1);
    sprite::Projectile* LightningBolt(ugdk::Vector2D &dir);
    sprite::Projectile* Fireball(ugdk::Vector2D &dir);

  private:
    utils::ImageFactory *factory_;
};

}

#endif /* HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_ */
