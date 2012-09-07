#include "mummy.h"

#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace component {

using namespace ugdk;
using scene::World;

COLLISION_DIRECT(Mummy*, MummyAntiStackCollision, voiddata) {
    sprite::WorldObject *obj = (sprite::WorldObject *) voiddata; 
    data_->MummyAntiStack(obj);
}

Mummy::Mummy(sprite::WorldObject* owner, double speed) 
    :   Creature(owner, speed) {

    Creature::AddKnownCollisions();
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummyAntiStackCollision(this));
}

Mummy::~Mummy() {
    WORLD()->DecreaseEnemyCount();
}

void Mummy::MummyAntiStack(sprite::WorldObject *obj) {
    Vector2D deviation = (owner_->world_position() - obj->world_position()).Normalize();
    walking_direction_ = (walking_direction_ + deviation*0.9).Normalize();
}

void Mummy::OnWorldAdd(scene::World* world) {
    world->IncreaseNumberOfEnemies();
}

}
