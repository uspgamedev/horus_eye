#include <cmath>
#include <iostream>

#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/time/timemanager.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/audio/audiomanager.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "game/components/logic/hero.h"

#include "game/components/controller.h"
#include "game/components/animation.h"
#include "game/components/caster.h"

#include "game/utils/imagefactory.h"
#include "game/components/logic/mummy.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

#include "game/scenes/world.h"

#include "game/skills/herobaseweapon.h"
#include "game/skills/usearguments.h"
#include "game/skills/skill.h"

#include <cmath>
#include <iostream>

#define INVUL_TIME 2000

using namespace std;
using namespace ugdk;
using namespace utils;
using resource::Resource;
using resource::Energy;
using resource::CountableResource;

namespace component {

#define SQRT_3 1.7320508075688772935274463415059

COLLISION_DIRECT(Hero*, MummySlowCollision, mummy) {
    data_->CollisionSlow();
}

Hero::Hero(sprite::WorldObject* owner, double speed)
    : Creature(owner, speed) {}

Hero::~Hero() {}

void Hero::CollisionSlow() {
   speed_ /= 1.19;
}

void Hero::SetupCollision() {
    if(owner_->collision_object()) {
        delete owner_->collision_object();
        owner_->set_collision_object(NULL);
    }
    owner_->set_collision_object(new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), owner_));
    owner_->collision_object()->InitializeCollisionClass("Hero");
    owner_->collision_object()->set_shape(new pyramidworks::geometry::Circle(0.3));
    AddKnownCollisions();
}

void Hero::AddKnownCollisions() {
    Creature::AddKnownCollisions();
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummySlowCollision(this));
}

}
