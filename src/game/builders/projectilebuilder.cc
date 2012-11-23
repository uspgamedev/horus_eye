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
#include "game/components/graphic.h"
#include "game/components/damageable.h"
#include "game/components/walker.h"
#include "game/components/statecontroller.h"
#include "game/components/shape.h"
#include "game/utils/imagefactory.h"
#include "game/scenes/world.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#define PI 3.1415926535897932384626433832795

namespace builder {

using namespace sprite;
using component::Walker;
using component::StateController;
using component::Animation;
using component::Shape;
using ugdk::base::ResourceManager;
using ugdk::Vector2D;
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
    component::StateController* controller = data_->component<component::StateController>();
    WorldObject* wall = static_cast<WorldObject*>(data);
    ugdk::Vector2D projectile_position = data_->world_position();
    ugdk::Vector2D wall_position = wall->world_position();
    ugdk::Vector2D new_direction = projectile_position - wall_position;
    double angle = new_direction.Angle();
    if( (angle >= PI/4 && angle <= 3*PI/4) || (angle <= -PI/4 && angle >= -3*PI/4) )
        controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::VERT));
    else
        controller->set_direction_vector(new_direction.Mirrored(ugdk::enums::mirroraxis::HORZ));

}

COLLISION_DIRECT(double, DamageCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    wobj->damageable()->TakeDamage(data_);
}

COLLISION_DIRECT(struct ObjectAndDamage, DamageAndDieCollision, obj) {
    WorldObject *wobj = static_cast<WorldObject*>(obj);
    if (!data_.obj->dead())
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
    wobj->AddComponent(new Shape(buildCollisionObject(wobj), NULL));
    wobj->shape()->collision()->set_shape(new pyramidworks::geometry::Circle(radius));
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
    WorldObject* wobj = buildObject(constants::GetInt("PROJECTILE_DURATION"), 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "magic_missile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, constants::GetInt("PROJECTILE_SPRITE_HEIGHT") + constants::GetDouble("PROJECTILE_HEIGHT")));
    wobj->graphic()->ChangeLightRadius(1.0);
    wobj->AddComponent(new Walker(wobj, constants::GetDouble("PROJECTILE_SPEED")));
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));

    struct ObjectAndDamage data(wobj, constants::GetInt("PROJECTILE_DAMAGE"));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(data));
    return wobj;
}

WorldObject* ProjectileBuilder::MagicBall(const Vector2D &dir) {
    WorldObject* wobj = buildObject(constants::GetInt("PROJECTILE_DURATION"), 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "magic_missile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, constants::GetInt("PROJECTILE_SPRITE_HEIGHT") + constants::GetDouble("PROJECTILE_HEIGHT")));
    wobj->graphic()->ChangeLightRadius(1.0);
    wobj->AddComponent(new Walker(wobj, constants::GetDouble("PROJECTILE_SPEED")));
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));

    struct ObjectAndDamage data(wobj, constants::GetInt("PROJECTILE_DAMAGE"));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(data));
    wobj->shape()->collision()->AddCollisionLogic("Wall", new BounceCollision(wobj));
    return wobj;
}
/***/
WorldObject* ProjectileBuilder::MummyProjectile(const ugdk::Vector2D &dir, double damage) {
    WorldObject* wobj = buildObject(constants::GetInt("PROJECTILE_DURATION"), 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( "mummy_projectile" ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, constants::GetInt("PROJECTILE_SPRITE_HEIGHT") + constants::GetDouble("PROJECTILE_HEIGHT")));
    wobj->graphic()->ChangeLightRadius(0.75);
    wobj->AddComponent(new Walker(wobj, constants::GetDouble("PROJECTILE_SPEED")));
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));

    struct ObjectAndDamage data(wobj, damage);
    wobj->shape()->collision()->AddCollisionLogic("Hero", new DamageAndDieCollision(data));

    return wobj;
}

WorldObject* ProjectileBuilder::LightningBolt(const Vector2D &dir) {
    WorldObject* wobj = buildObject(constants::GetInt("LIGHTNING_DURATION"), 0.25);
    wobj->AddComponent(new component::Animation(wobj, "lightning_bolt", lightning_animation_));
    wobj->component<Animation>()->ChangeDirection(GetFromScreenVector(dir));
    wobj->component<Animation>()->ChangeAnimation(utils::ATTACK);
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, constants::GetDouble("LIGHTNING_SPRITE_HEIGHT")));
    wobj->graphic()->ChangeLightRadius(1.0);
    wobj->AddComponent(new Walker(wobj, constants::GetDouble("LIGHTNING_SPEED")));
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageCollision(constants::GetInt("LIGHTNING_DAMAGE")));

    return wobj;
}

WorldObject* ProjectileBuilder::Fireball(const Vector2D &dir) {
    builder::ExplosionBuilder builder;
    WorldObject *explosion = builder.FireballExplosion();

    WorldObject* wobj = buildObject(constants::GetInt("FIREBALL_DURATION"), 0.25);
    wobj->AddComponent(new component::Animation(wobj, "fireball", fireball_animation_));
    wobj->component<Animation>()->ChangeDirection(GetFromScreenVector(dir));
    wobj->component<Animation>()->ChangeAnimation(utils::ATTACK);
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, constants::GetDouble("FIREBALL_SPRITE_HEIGHT")));
    wobj->graphic()->ChangeLightRadius(1.0);
    // Give the light an orange color
    wobj->node()->light()->set_color(ugdk::Color(1.0, 0.521568, 0.082352));
    wobj->set_start_to_die_callback(Carrier(explosion));
    wobj->AddComponent(new Walker(wobj, constants::GetDouble("FIREBALL_SPEED")));
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));

    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DieCollision(wobj));
    return wobj;
}

}
