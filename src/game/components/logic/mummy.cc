#include "mummy.h"

#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace component {

using namespace ugdk;
using scene::World;

Mummy::Mummy(sprite::WorldObject* owner, double speed) 
    :   Creature(owner, speed) {}

Mummy::~Mummy() {}

void Mummy::OnWorldAdd(scene::World* world) {
    world->IncreaseNumberOfEnemies();
}

}
