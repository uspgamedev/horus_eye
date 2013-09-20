#ifndef HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace builder {
namespace ProjectileBuilder {

    void PrepareProjectile(const sprite::WObjPtr&, const ugdk::math::Vector2D &dir, double speed);

sprite::WObjPtr MagicMissile(const ugdk::math::Vector2D &dir);
sprite::WObjPtr MagicBall(const ugdk::math::Vector2D &dir);
sprite::WObjPtr MummyProjectile(const ugdk::math::Vector2D &dir, double damage = 100.0);
sprite::WObjPtr LightningBolt(const ugdk::math::Vector2D &dir);
sprite::WObjPtr Fireball(const ugdk::math::Vector2D &dir);

} // namespace ProjectileBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_ */
