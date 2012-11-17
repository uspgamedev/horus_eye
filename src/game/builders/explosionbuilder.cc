#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
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

#include "explosionbuilder.h"

#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/logic/follower.h"
#include "game/components/shape.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"
#include "game/constants.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {

using std::tr1::bind;
using ugdk::action::AnimationSet;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using pyramidworks::collision::CollisionObject;
using component::Follower;
using component::Shape;
using sprite::WorldObject;

COLLISION_DIRECT(double, ExplosionCollision, obj) {
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
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

WorldObject* ExplosionBuilder::FireballExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.ExplosionImage(), "HERO_FIREBALL_WEAPON");
    
    wobj->graphic()->ChangeLightRadius(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"));
    wobj->graphic()->node()->light()->set_color(ugdk::Color(1.0, 0.521568, 0.082352));

    CollisionObject* col = wobj->shape()->collision();
    col->AddCollisionLogic("Mummy", new ExplosionCollision(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE")));
    col->set_shape(new pyramidworks::geometry::Circle(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));

    return wobj;
}

WorldObject* ExplosionBuilder::EarthquakeExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.QuakeImage(), "HERO_EXPLOSION_WEAPON");

    wobj->graphic()->ChangeLightRadius(1.3* constants::GetDouble("QUAKE_EXPLOSION_RADIUS"));

    CollisionObject* col = wobj->shape()->collision();
    col->AddCollisionLogic("Mummy", new ExplosionCollision(constants::GetInt("QUAKE_EXPLOSION_DAMAGE")));
    col->set_shape(new pyramidworks::geometry::Circle(constants::GetDouble("QUAKE_EXPLOSION_RADIUS")));

    return wobj;
}

WorldObject* ExplosionBuilder::MeteorExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion(factory.ExplosionImage(), "HERO_FIREBALL_WEAPON");

    static double explosion_fireball_ratio = (constants::GetDouble("METEOR_EXPLOSION_RADIUS") / constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"));
    wobj->node()->modifier()->set_scale(Vector2D(explosion_fireball_ratio));
    //explosion->set_hotspot(explosion->hotspot() * explosion_fireball_ratio); Oh noes TODO fix hotspot

    CollisionObject* col = wobj->shape()->collision();
    col->AddCollisionLogic("Mummy", new ExplosionCollision(constants::GetInt("METEOR_EXPLOSION_DAMAGE")));
    col->set_shape(new pyramidworks::geometry::Circle(constants::GetDouble("METEOR_EXPLOSION_RADIUS")));

    return wobj;
}

}
