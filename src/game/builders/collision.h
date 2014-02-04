#ifndef HORUSEYE_BUILDERS_COLLISION_H_
#define HORUSEYE_BUILDERS_COLLISION_H_

#include <pyramidworks/collision.h>
#include "game/constants.h"
#include "game/sprites.h"

namespace builder {

pyramidworks::collision::CollisionLogic DieCollision(const sprite::WObjWeakPtr& owner);
pyramidworks::collision::CollisionLogic DamageCollision(double damage);
pyramidworks::collision::CollisionLogic DamageCollision(const std::string& constant_name);
pyramidworks::collision::CollisionLogic DamageAndDieCollision(const sprite::WObjWeakPtr& owner, double damage);
pyramidworks::collision::CollisionLogic DamageAndDieCollision(const sprite::WObjWeakPtr& owner, const std::string& constant_name);
pyramidworks::collision::CollisionLogic BounceCollision(const sprite::WObjWeakPtr& owner);

} // namespace builder

#endif // HORUSEYE_BUILDERS_COLLISION_H_