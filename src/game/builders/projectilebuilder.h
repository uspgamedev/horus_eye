#ifndef HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace builder {
namespace ProjectileBuilder {

sprite::WorldObject* MagicMissile(const ugdk::math::Vector2D &dir);
sprite::WorldObject* MagicBall(const ugdk::math::Vector2D &dir);
sprite::WorldObject* MummyProjectile(const ugdk::math::Vector2D &dir, double damage = 100.0);
sprite::WorldObject* LightningBolt(const ugdk::math::Vector2D &dir);
sprite::WorldObject* Fireball(const ugdk::math::Vector2D &dir);

} // namespace ProjectileBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_PROJECTILEBUILDER_H_ */
