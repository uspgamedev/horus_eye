#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/animationset.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "projectilebuilder.h"

#include "game/builders/explosionbuilder.h"
#include "game/builders/functions/carrier.h"
#include "game/components/animation.h"
#include "game/components/damageable.h"
#include "game/components/projectile.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"
#include "game/scenes/world.h"
#include "game/utils/isometricanimationset.h"

#define PI 3.1415926535897932384626433832795

namespace builder {

using namespace sprite;
using component::Projectile;
using ugdk::base::ResourceManager;
using ugdk::Vector2D;
using utils::Constants;
using utils::IsometricAnimationSet;
using sprite::WorldObject;
using pyramidworks::collision::CollisionObject;
using function::Carrier;

component::Direction GetFromScreenVector(const Vector2D& dir) {
    return component::Direction::FromScreenVector(scene::World::FromWorldLinearCoordinates(dir));
}

utils::IsometricAnimationSet* ProjectileBuilder::fireball_animation_ = NULL;
utils::IsometricAnimationSet* ProjectileBuilder::lightning_animation_ = NULL;

struct ObjectAndDamage {
    ObjectAndDamage(WorldObject *object, double dmg)
        : obj(object), damage(dmg) {}

    WorldObject *obj;
    double damage;
};

COLLISION_DIRECT(WorldObject*, DieCollision, data) { 
    data_->StartToDie();
}

COLLISION_DIRECT(WorldObject*, BounceCollision, data) {
    component::Projectile* projectile_logic = dynamic_cast<component::Projectile*>(data_->logic());
    WorldObject* wall = static_cast<WorldObject*>(data);
    ugdk::Vector2D projectile_position = data_->world_position();
    ugdk::Vector2D wall_position = wall->world_position();
    ugdk::Vector2D new_direction = projectile_position - wall_position;
    double angle = new_direction.Angle();
    if( (angle >= PI/4 && angle <= 3*PI/4) || (angle <= -PI/4 && angle >= -3*PI/4) )
        projectile_logic->set_direction(new_direction.Mirrored(ugdk::enums::mirroraxis::VERT));
    else
        projectile_logic->set_direction(new_direction.Mirrored(ugdk::enums::mirroraxis::HORZ));

}

COLLISION_DIRECT(double, DamageCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    wobj->damageable()->TakeDamage(data_);
}

COLLISION_DIRECT(struct ObjectAndDamage, DamageAndDieCollision, obj) {
    WorldObject *wobj = static_cast<WorldObject*>(obj);
    if (data_.obj->is_active())
        wobj->damageable()->TakeDamage(data_.damage);
    data_.obj->StartToDie();
}

static CollisionObject* buildCollisionObject(WorldObject* wobj) {
    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Projectile");
    col->AddCollisionLogic("Wall", new DieCollision(wobj));
    return col;
}

static WorldObject* buildObject(double duration, double radius) {
    WorldObject* wobj = new WorldObject(duration);
    wobj->set_collision_object(buildCollisionObject(wobj));
    wobj->set_shape(new pyramidworks::geometry::Circle(radius));
    return wobj;
}

void ProjectileBuilder::InitializeAnimations() {
    if(fireball_animation_ == NULL)
        fireball_animation_ = new IsometricAnimationSet(
            ResourceManager::GetAnimationSetFromFile("animations/fireball.gdd"));

    if(lightning_animation_ == NULL)
        lightning_animation_ = new IsometricAnimationSet(
            ResourceManager::GetAnimationSetFromFile("animations/lightning.gdd"));
}

WorldObject* ProjectileBuilder::MagicMissile(const Vector2D &dir) {
    WorldObject* wobj = buildObject(Constants::PROJECTILE_DURATION, 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "magic_missile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT));
    wobj->set_light_radius(1.0);
    wobj->set_logic(new Projectile(wobj, Constants::PROJECTILE_SPEED, dir));

    struct ObjectAndDamage data(wobj, Constants::PROJECTILE_DAMAGE);
    wobj->collision_object()->AddCollisionLogic("Mummy", new DamageAndDieCollision(data));
    return wobj;
}

WorldObject* ProjectileBuilder::MagicBall(const Vector2D &dir) {
    WorldObject* wobj = buildObject(Constants::PROJECTILE_DURATION, 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "magic_missile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT));
    wobj->set_light_radius(1.0);
    wobj->set_logic(new Projectile(wobj, Constants::PROJECTILE_SPEED, dir));

    struct ObjectAndDamage data(wobj, Constants::PROJECTILE_DAMAGE);
    wobj->collision_object()->AddCollisionLogic("Mummy", new DamageAndDieCollision(data));
    wobj->collision_object()->AddCollisionLogic("Wall", new BounceCollision(wobj));
    return wobj;
}

WorldObject* ProjectileBuilder::MummyProjectile(const ugdk::Vector2D &dir, int damage) {
    WorldObject* wobj = buildObject(Constants::PROJECTILE_DURATION, 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "mummy_projectile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT));
    wobj->set_light_radius(0.75);
    wobj->set_logic(new Projectile(wobj, Constants::PROJECTILE_SPEED, dir));

    struct ObjectAndDamage data(wobj, damage);
    wobj->collision_object()->AddCollisionLogic("Hero", new DamageAndDieCollision(data));

    return wobj;
}

WorldObject* ProjectileBuilder::LightningBolt(const Vector2D &dir) {
    WorldObject* wobj = buildObject(Constants::LIGHTNING_DURATION, 0.25);
    wobj->set_animation(new component::Animation(wobj, "lightning_bolt", lightning_animation_));
    wobj->animation()->set_direction(GetFromScreenVector(dir));
    wobj->animation()->set_animation(utils::ATTACKING);
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::LIGHTNING_SPRITE_HEIGHT));
    wobj->set_light_radius(1.0);
    wobj->set_logic(new Projectile(wobj, Constants::LIGHTNING_SPEED, dir));
    wobj->collision_object()->AddCollisionLogic("Mummy", new DamageCollision(Constants::LIGHTNING_DAMAGE));
    return wobj;
}

WorldObject* ProjectileBuilder::Fireball(const Vector2D &dir) {
    builder::ExplosionBuilder builder;
    WorldObject *explosion = builder.FireballExplosion();

    WorldObject* wobj = buildObject(Constants::FIREBALL_DURATION, 0.25);
    wobj->set_animation(new component::Animation(wobj, "fireball", fireball_animation_));
    wobj->animation()->set_direction(GetFromScreenVector(dir));
    wobj->animation()->set_animation(utils::ATTACKING);
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::FIREBALL_SPRITE_HEIGHT));
    wobj->set_light_radius(1.0);
    // Give the light an orange color
    wobj->node()->light()->set_color(ugdk::Color(1.0, 0.521568, 0.082352));
    wobj->set_start_to_die_callback(Carrier(explosion));
    wobj->set_logic(new Projectile(wobj, Constants::FIREBALL_SPEED, dir));
    wobj->collision_object()->AddCollisionLogic("Mummy", new DieCollision(wobj));
    return wobj;
}

}
