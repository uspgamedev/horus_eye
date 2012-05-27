#include <functional>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>

#include "entitybuilder.h"

#include "game/components/damageable.h"
#include "game/components/logic/follower.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

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

static WorldObject* baseExplosion(ugdk::graphic::Spritesheet* sheet, const std::string& anim) {
    AnimationSet* set = ResourceManager::GetAnimationSetFromFile("animations/explosion.gdd");
    
    WorldObject *wobj = new WorldObject;

    Sprite* sprite = new Sprite(sheet, set);
    sprite->SelectAnimation(anim);
    sprite->AddTickFunctionToAnimation(bind(&WorldObject::StartToDie, wobj));
    wobj->node()->set_drawable(sprite);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Explosion");
    wobj->set_collision_object(col);

    return wobj;
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

WorldObject* EntityBuilder::FireballExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.ExplosionImage(), "HERO_FIREBALL_WEAPON");
    
    wobj->set_light_radius(1.3*Constants::FIREBALL_EXPLOSION_RADIUS);
    wobj->node()->light()->set_color(ugdk::Color(1.0, 0.521568, 0.082352));

    CollisionObject* col = wobj->collision_object();
    col->AddCollisionLogic("Mummy", new DamageCollisionExtra(Constants::FIREBALL_EXPLOSION_DAMAGE));
    col->set_shape(new pyramidworks::geometry::Circle(Constants::FIREBALL_EXPLOSION_RADIUS));

    return wobj;
}

WorldObject* EntityBuilder::EarthquakeExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.QuakeImage(), "HERO_EXPLOSION_WEAPON");

    wobj->set_light_radius(1.3*Constants::QUAKE_EXPLOSION_RADIUS);

    CollisionObject* col = wobj->collision_object();
    col->AddCollisionLogic("Mummy", new DamageCollisionExtra(Constants::QUAKE_EXPLOSION_DAMAGE));
    col->set_shape(new pyramidworks::geometry::Circle(Constants::QUAKE_EXPLOSION_RADIUS));

    return wobj;
}

WorldObject* EntityBuilder::MeteorExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.ExplosionImage(), "HERO_FIREBALL_WEAPON");

    static double explosion_fireball_ratio = (Constants::METEOR_EXPLOSION_RADIUS / Constants::FIREBALL_EXPLOSION_RADIUS);
    wobj->node()->modifier()->set_scale(Vector2D(explosion_fireball_ratio));
    //explosion->set_hotspot(explosion->hotspot() * explosion_fireball_ratio); Oh noes TODO fix hotspot

    CollisionObject* col = wobj->collision_object();
    col->AddCollisionLogic("Mummy", new DamageCollisionExtra(Constants::METEOR_EXPLOSION_DAMAGE));
    col->set_shape(new pyramidworks::geometry::Circle(Constants::METEOR_EXPLOSION_RADIUS));

    return wobj;
}

}
