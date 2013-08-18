#include "game/builders/collision.h"

#include <cmath>
#include "game/constants.h"
#include "game/components/damageable.h"
#include "game/components/statecontroller.h"
#include "game/sprites/worldobject.h"

#define PI 3.1415926535897932384626433832795

namespace builder {

using ugdk::math::Vector2D;
using ugdk::action::Entity;
using pyramidworks::collision::CollisionLogic;
using sprite::WorldObject;

CollisionLogic DieCollision(sprite::WorldObject* owner) {
    return [owner](Entity*) {
        owner->Die();
    };
}

CollisionLogic DamageCollision(double damage) {
    return [damage](Entity* obj) {
        WorldObject *wobj = dynamic_cast<WorldObject *>(obj);
        if(wobj && wobj->damageable()) 
            wobj->damageable()->TakeDamage(damage);
    };
}

CollisionLogic DamageCollision(const std::string& constant_name) {
    return DamageCollision(constants::GetDouble(constant_name));
}

CollisionLogic DamageAndDieCollision(WorldObject* owner, double damage) {
    return [owner,damage](Entity* obj) {
        WorldObject *wobj = dynamic_cast<WorldObject *>(obj);
        if(wobj && !owner->dead() && wobj->damageable()) {
            wobj->damageable()->TakeDamage(damage);
            owner->Die();
        }
    };
}

CollisionLogic DamageAndDieCollision(WorldObject* owner, const std::string& constant_name) {
    return DamageAndDieCollision(owner, constants::GetDouble(constant_name));
}

pyramidworks::collision::CollisionLogic BounceCollision(sprite::WorldObject* owner) {
    return [owner](Entity* obj) {
        component::StateController* controller = owner->component<component::StateController>();
        WorldObject* wall = dynamic_cast<WorldObject*>(obj);
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
