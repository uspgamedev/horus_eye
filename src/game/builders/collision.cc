#include "game/builders/collision.h"

#include <cmath>
#include <pyramidworks/collision/collisiondata.h>
#include <pyramidworks/collision/collisionobject.h>
#include "game/constants.h"
#include "game/components/damageable.h"
#include "game/components/statecontroller.h"
#include "game/sprites/worldobject.h"

#define PI 3.1415926535897932384626433832795

namespace builder {

using ugdk::math::Vector2D;
using pyramidworks::collision::CollisionLogic;
using pyramidworks::collision::CollisionObject;
using sprite::WorldObject;

CollisionLogic DieCollision(const sprite::WObjWeakPtr& owner) {
    return [owner](const CollisionObject*) {
        if(auto o = owner.lock())
            o->damageable()->Die();
    };
}

CollisionLogic DamageCollision(double damage) {
    return [damage](const CollisionObject* obj) {
        WorldObject* wobj = dynamic_cast<WorldObject*>(obj->data());
        if(wobj && wobj->damageable()) 
            wobj->damageable()->TakeDamage(damage);
    };
}

CollisionLogic DamageCollision(const std::string& constant_name) {
    return DamageCollision(constants::GetDouble(constant_name));
}

CollisionLogic DamageAndDieCollision(const sprite::WObjWeakPtr& ownerweak, double damage) {
    return [ownerweak,damage](const CollisionObject* obj) {
        auto owner = ownerweak.lock();
        WorldObject* wobj = dynamic_cast<WorldObject*>(obj->data());
        if(wobj && owner && !owner->damageable()->dead() && wobj->damageable()) {
            wobj->damageable()->TakeDamage(damage);
            owner->damageable()->Die();
        }
    };
}

CollisionLogic DamageAndDieCollision(const sprite::WObjWeakPtr& owner, const std::string& constant_name) {
    return DamageAndDieCollision(owner, constants::GetDouble(constant_name));
}

pyramidworks::collision::CollisionLogic BounceCollision(const sprite::WObjWeakPtr& ownerweak) {
    return [ownerweak](const CollisionObject* obj) {
        auto owner = ownerweak.lock();
        if(!owner) return;
        component::StateController* controller = owner->component<component::StateController>();
        WorldObject* wall = dynamic_cast<WorldObject*>(obj->data());
        if(!controller || !wall) return;
        ugdk::math::Vector2D projectile_position = owner->world_position();
        ugdk::math::Vector2D wall_position = wall->world_position();
        ugdk::math::Vector2D new_direction = projectile_position - wall_position;
        double angle = new_direction.Angle();
        if( (angle >= PI/4 && angle <= 3*PI/4) || (angle <= -PI/4 && angle >= -3*PI/4) )
            controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::VERT));
        else
            controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::HORZ));
    };
}


} // namespace builder
