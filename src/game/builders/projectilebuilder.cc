#include "projectilebuilder.h"


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
using ugdk::graphic::Sprite;
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

COLLISION_DIRECT(WorldObject*, DieCollision, data) { data_->StartToDie(); }

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

class DamageCollision : public pyramidworks::collision::CollisionLogic {
  public:
    DamageCollision(double damage) : damage_(damage) {}
    DamageCollision(const std::string& constant_name) : damage_(constants::GetDouble(constant_name)) {}

    void Handle(void* obj) {
        WorldObject *wobj = static_cast<WorldObject *>(obj);
        if(wobj->damageable()) wobj->damageable()->TakeDamage(damage_);
    }

  private:
    double damage_;
};

class DamageAndDieCollision : public pyramidworks::collision::CollisionLogic {
  public:
    DamageAndDieCollision(WorldObject* owner, double damage) : owner_(owner), damage_(damage) {}
    DamageAndDieCollision(WorldObject* owner, const std::string& constant_name) : owner_(owner), damage_(constants::GetDouble(constant_name)) {}

    void Handle(void* obj) {
        WorldObject *wobj = static_cast<WorldObject *>(obj);
        if(!owner_->dead() && wobj->damageable()) {
            wobj->damageable()->TakeDamage(damage_);
            owner_->StartToDie();
        }
    }

  private:
    double damage_;
    WorldObject* owner_;
};

static CollisionObject* buildCollisionObject(WorldObject* wobj, double radius) {
    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Projectile");
    col->set_shape(new pyramidworks::geometry::Circle(radius));
    col->AddCollisionLogic("Wall", new DieCollision(wobj));
    return col;
}

static WorldObject* buildProjectile(const ugdk::Vector2D &dir, double speed, double duration, double radius) {
    WorldObject* wobj = new WorldObject(duration);
    wobj->AddComponent(new StateController(component::Direction::FromWorldVector(dir), dir));
    wobj->AddComponent(new Walker(wobj, speed));
    wobj->AddComponent(new Shape(buildCollisionObject(wobj, radius), NULL));
    return wobj;
}

void ProjectileBuilder::InitializeAnimations() {
    fireball_animation_ = IsometricAnimationSet::LoadFromResourceManager("animations/fireball.gdd");
    lightning_animation_ = IsometricAnimationSet::LoadFromResourceManager("animations/lightning.gdd");
}

WorldObject* ProjectileBuilder::MagicMissile(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, constants::GetDouble("PROJECTILE_SPEED"), constants::GetDouble("PROJECTILE_DURATION"), 0.15);
    wobj->AddComponent(new component::BaseGraphic(new Sprite("magic_missile"), 1.0));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(wobj, "PROJECTILE_DAMAGE"));
    return wobj;
}

WorldObject* ProjectileBuilder::MagicBall(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, constants::GetDouble("PROJECTILE_SPEED"), constants::GetDouble("PROJECTILE_DURATION"), 0.15);
    wobj->AddComponent(new component::BaseGraphic(new Sprite("magic_missile"), 1.0));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(wobj, "PROJECTILE_DAMAGE"));
    wobj->shape()->collision()->AddCollisionLogic("Wall", new BounceCollision(wobj));
    return wobj;
}
/***/
WorldObject* ProjectileBuilder::MummyProjectile(const ugdk::Vector2D &dir, double damage) {
    WorldObject* wobj = buildProjectile(dir, constants::GetDouble("PROJECTILE_SPEED"), constants::GetDouble("PROJECTILE_DURATION"), 0.15);
    wobj->AddComponent(new component::BaseGraphic(new Sprite("mummy_projectile"), 0.75));
    wobj->shape()->collision()->AddCollisionLogic("Hero", new DamageAndDieCollision(wobj, damage));
    return wobj;
}

WorldObject* ProjectileBuilder::LightningBolt(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, constants::GetDouble("LIGHTNING_SPEED"), constants::GetDouble("LIGHTNING_DURATION"), 0.25);
    wobj->AddComponent(new component::Graphic("lightning_bolt", lightning_animation_, 1.0));
    wobj->AddComponent(new component::Animation(wobj));
    wobj->component<Animation>()->ChangeDirection(GetFromScreenVector(dir));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageCollision(constants::GetDouble("LIGHTNING_DAMAGE")));
    return wobj;
}

WorldObject* ProjectileBuilder::Fireball(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, constants::GetDouble("FIREBALL_SPEED"), constants::GetDouble("FIREBALL_DURATION"), 0.25);
    wobj->AddComponent(new component::Graphic("fireball", fireball_animation_, 1.0));
    wobj->graphic()->node()->light()->set_color(ugdk::Color(1.0, 0.521568, 0.082352)); // Orange
    wobj->AddComponent(new component::Animation(wobj));
    wobj->component<Animation>()->ChangeDirection(GetFromScreenVector(dir));
    wobj->set_start_to_die_callback(Carrier(builder::ExplosionBuilder::FireballExplosion()));

    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DieCollision(wobj));
    return wobj;
}

}
