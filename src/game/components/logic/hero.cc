#include "game/components/logic/hero.h"

#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace component {

COLLISION_DIRECT(Hero*, MummySlowCollision, mummy) {
    data_->speed_ /= 1.19;
}

void Hero::SetupCollision() {
    if(owner_->collision_object()) {
        delete owner_->collision_object();
        owner_->set_collision_object(NULL);
    }
    owner_->set_collision_object(new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), owner_));
    owner_->collision_object()->InitializeCollisionClass("Hero");
    owner_->collision_object()->set_shape(new pyramidworks::geometry::Circle(0.3));
    owner_->collision_object()->AddCollisionLogic("Wall", CreateCreatureRectCollision(this));
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummySlowCollision(this));
}

}
