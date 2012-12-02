#include "projectilebuilder.h"

#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/base/types.h>
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
#include "game/scenes/world.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#define PI 3.1415926535897932384626433832795

namespace builder {
namespace ProjectileBuilder {

using component::Direction;
using ugdk::base::ResourceManager;
using ugdk::Vector2D;
using utils::IsometricAnimationSet;
using sprite::WorldObject;
using pyramidworks::collision::CollisionObject;
using function::Carrier;

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

static WorldObject* buildProjectile(const ugdk::Vector2D &dir, const std::string& spritesheet, const std::string& isometric_animation, 
                                    double light_radius, double speed, double duration, double radius) {

    IsometricAnimationSet* set = isometric_animation.empty() ? NULL : IsometricAnimationSet::LoadFromResourceManager(isometric_animation);
    WorldObject* wobj = new WorldObject(duration);
    wobj->AddComponent(new component::Graphic(spritesheet, set, light_radius));
    wobj->AddComponent(new component::StateController(Direction::FromWorldVector(dir), dir));
    wobj->AddComponent(new component::Walker(wobj, speed));
    wobj->AddComponent(new component::Shape(buildCollisionObject(wobj, radius), NULL));
    return wobj;
}

WorldObject* MagicMissile(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICMISSILE_SPEED"), constants::GetDouble("MAGICMISSILE_DURATION"), 0.15);
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(wobj, "MAGICMISSILE_DAMAGE"));
    return wobj;
}

WorldObject* MagicBall(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICBALL_SPEED"), constants::GetDouble("MAGICBALL_DURATION"), 0.15);
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageAndDieCollision(wobj, "MAGICBALL_DAMAGE"));
    wobj->shape()->collision()->AddCollisionLogic("Wall", new BounceCollision(wobj));
    return wobj;
}
/***/
WorldObject* MummyProjectile(const ugdk::Vector2D &dir, double damage) {
    WorldObject* wobj = buildProjectile(dir, "mummy_projectile", "", 0.75, constants::GetDouble("MUMMYPROJECTILE_SPEED"), constants::GetDouble("MUMMYPROJECTILE_DURATION"), 0.15);
    wobj->shape()->collision()->AddCollisionLogic("Hero", new DamageAndDieCollision(wobj, damage));
    return wobj;
}

WorldObject* LightningBolt(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "lightning_bolt", "animations/lightning.gdd", 1.0, constants::GetDouble("LIGHTNING_SPEED"), constants::GetDouble("LIGHTNING_DURATION"), 0.25);
    wobj->AddComponent(new component::Animation(wobj, utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DamageCollision("LIGHTNING_DAMAGE"));
    wobj->set_start_to_die_callback(std::tr1::mem_fn(&WorldObject::Die));
    return wobj;
}

WorldObject* Fireball(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "fireball", "animations/fireball.gdd", 1.0, constants::GetDouble("FIREBALL_SPEED"), constants::GetDouble("FIREBALL_DURATION"), 0.25);
    wobj->graphic()->ChangeLightColor(ugdk::Color(1.0, 0.521568, 0.082352)); // Orange
    wobj->AddComponent(new component::Animation(wobj, utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->shape()->collision()->AddCollisionLogic("Mummy", new DieCollision(wobj));
    wobj->set_start_to_die_callback(Carrier(builder::ExplosionBuilder::FireballExplosion()));
    return wobj;
}

} // namespace ProjectileBuilder
} // namespace builder
