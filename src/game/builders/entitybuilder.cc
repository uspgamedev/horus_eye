#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/genericcollisionlogic.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "entitybuilder.h"

#include "game/components/damageable.h"
#include "game/components/shape.h"
#include "game/components/graphic.h"
#include "game/components/logic/follower.h"
#include "game/components/logic/wall.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"

namespace builder {

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::action::AnimationSet;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::GenericCollisionLogic;
using component::Follower;
using component::Shape;
using sprite::WorldObject;

static void DamageCollisionFunc(double damage, void* obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(damage);
}


WorldObject* EntityBuilder::BlueShieldEntity(sprite::WorldObject *target) {
    AnimationSet* set = ResourceManager::GetAnimationSetFromFile("animations/shield.gdd");
    utils::ImageFactory img;

    Sprite* sprite = new Sprite(img.ShieldImage(), set);
    sprite->SelectAnimation("IDLE");
    
    WorldObject* wobj = new WorldObject(30.0);
    wobj->AddComponent(new component::Graphic);
    wobj->node()->set_drawable(sprite);
    wobj->AddComponent(new Follower(wobj, target));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Projectile");
    col->AddCollisionLogic("Mummy", new GenericCollisionLogic(bind(DamageCollisionFunc, 75, _1)));
    col->set_shape(new pyramidworks::geometry::Circle(0.80));
    
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

}
