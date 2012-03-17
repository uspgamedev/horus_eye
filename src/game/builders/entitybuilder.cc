#include <ugdk/time/timeaccumulator.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>

#include "entitybuilder.h"

#include "game/sprites/creatures/creature.h"
#include "game/sprites/follower.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)


namespace builder {

using sprite::Creature;
using sprite::Follower;
using pyramidworks::collision::CollisionObject;

COLLISION_DIRECT(double, DamageCollisionExtra, obj) {
	Creature *creature = (Creature *) obj;
    creature->TakeDamage(data_);
}

sprite::WorldObject* EntityBuilder::BlueShieldEntity(sprite::WorldObject *target) {
    // Creating a new Sprite with the shield image
    ugdk::AnimationSet* set = RESOURCE_MANAGER()->animation_loader().Load("animations/shield.gdd");
    utils::ImageFactory img;
    ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite(img.ShieldImage(), set);
    sprite->SelectAnimation("IDLE");
    
    Follower* follower = new Follower(target, 30.0);
    follower->node()->set_drawable(sprite);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), follower);
    col->InitializeCollisionClass("Projectile");
    col->AddCollisionLogic("Mummy", new DamageCollisionExtra(75.0));
    col->set_shape(new pyramidworks::geometry::Circle(0.80));

    follower->set_collision_object(col);
    return follower;
}

}
