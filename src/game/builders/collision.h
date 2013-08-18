#ifndef HORUSEYE_BUILDERS_COLLISION_H_
#define HORUSEYE_BUILDERS_COLLISION_H_

#include <pyramidworks/collision.h>
#include "game/constants.h"
#include "game/sprites.h"

namespace builder {

pyramidworks::collision::CollisionLogic DieCollision(sprite::WorldObject* owner);
pyramidworks::collision::CollisionLogic DamageCollision(double damage);
pyramidworks::collision::CollisionLogic DamageCollision(const std::string& constant_name);
pyramidworks::collision::CollisionLogic DamageAndDieCollision(sprite::WorldObject* owner, double damage);
pyramidworks::collision::CollisionLogic DamageAndDieCollision(sprite::WorldObject* owner, const std::string& constant_name);
pyramidworks::collision::CollisionLogic BounceCollision(sprite::WorldObject* owner);

} // namespace builder

#endif // HORUSEYE_BUILDERS_COLLISION_H_