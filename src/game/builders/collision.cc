#include "game/builders/collision.h"

#include <cmath>
#include "game/components/damageable.h"
#include "game/components/statecontroller.h"
#include "game/sprites/worldobject.h"

#define PI 3.1415926535897932384626433832795

namespace builder {

using ugdk::math::Vector2D;
using sprite::WorldObject;

void DieCollision::Handle(void*) {
    owner_->Die();
}

void BounceCollision::Handle(void* data) {
    component::StateController* controller = owner_->component<component::StateController>();
    WorldObject* wall = static_cast<WorldObject*>(data);
    ugdk::math::Vector2D projectile_position = owner_->world_position();
    ugdk::math::Vector2D wall_position = wall->world_position();
    ugdk::math::Vector2D new_direction = projectile_position - wall_position;
    double angle = new_direction.Angle();
    if( (angle >= PI/4 && angle <= 3*PI/4) || (angle <= -PI/4 && angle >= -3*PI/4) )
        controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::VERT));
    else
        controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::HORZ));
}

void DamageCollision::Handle(void* obj) {
    WorldObject *wobj = static_cast<WorldObject *>(obj);
    if(wobj->damageable()) wobj->damageable()->TakeDamage(damage_);
}

void DamageAndDieCollision::Handle(void* obj) {
    WorldObject *wobj = static_cast<WorldObject *>(obj);
    if(!owner_->dead() && wobj->damageable()) {
        wobj->damageable()->TakeDamage(damage_);
        owner_->Die();
    }
}

} // namespace builder
