#include <functional>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "entitybuilder.h"

#include "game/components/damageable.h"
#include "game/components/logic/follower.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"

namespace builder {

using std::tr1::bind;
using ugdk::action::AnimationSet;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using pyramidworks::collision::CollisionObject;
using component::Creature;
using component::Follower;
using sprite::WorldObject;
using utils::Constants;

COLLISION_DIRECT(double, DamageCollisionExtra, obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(data_);
}

COLLISION_DIRECT(scene::World*, WinCollision, obj) {
    if (data_->CountRemainingEnemies() == 0 && data_->num_button_not_pressed().Get() == 0)
        data_->FinishLevel(utils::LevelManager::FINISH_WIN);
}

WorldObject* EntityBuilder::BlueShieldEntity(sprite::WorldObject *target) {
    AnimationSet* set = ResourceManager::GetAnimationSetFromFile("animations/shield.gdd");
    utils::ImageFactory img;

    Sprite* sprite = new Sprite(img.ShieldImage(), set);
    sprite->SelectAnimation("IDLE");
    
    WorldObject* wobj = new WorldObject(30.0);
    wobj->node()->set_drawable(sprite);
    wobj->set_logic(new Follower(wobj, target));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Projectile");
    col->AddCollisionLogic("Mummy", new DamageCollisionExtra(75.0));
    col->set_shape(new pyramidworks::geometry::Circle(0.80));
    wobj->set_collision_object(col);

    return wobj;
}



WorldObject* EntityBuilder::Door(scene::World* world) {
    WorldObject* wobj = new WorldObject;
    wobj->node()->set_drawable(new Sprite(world->image_factory()->DoorImage()));

    CollisionObject* col = new CollisionObject(world->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->AddCollisionLogic("Hero", new WinCollision(world));
    col->set_shape(new pyramidworks::geometry::Rect(Constants::DOOR_BOUND_WIDTH, Constants::DOOR_BOUND_HEIGHT));
    wobj->set_collision_object(col);

    return wobj;
}

}