#include "projectilebuilder.h"

#include <cmath>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/structure/types.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/builders/collision.h"
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

namespace builder {
namespace ProjectileBuilder {

using component::Direction;
using ugdk::math::Vector2D;
using utils::IsometricAnimationSet;
using sprite::WorldObject;
using pyramidworks::collision::CollisionObject;
using function::Carrier;

static CollisionObject* buildCollisionObject(WorldObject* wobj, double radius) {
    CollisionObject* col = new CollisionObject(wobj, "Projectile");
    col->set_shape(new pyramidworks::geometry::Circle(radius));
    col->AddCollisionLogic("Wall", DieCollision(wobj));
    return col;
}

void PrepareProjectile(sprite::WorldObject* wobj, const ugdk::math::Vector2D &dir, double speed) {
    wobj->AddComponent(new component::StateController(Direction::FromWorldVector(dir), dir));
    wobj->AddComponent(new component::Walker(wobj, speed));
}

static WorldObject* buildProjectile(const ugdk::math::Vector2D &dir, const std::string& spritesheet, const std::string& isometric_animation, 
                                    double light_radius, double speed, double duration) {

    IsometricAnimationSet* set = isometric_animation.empty() ? NULL : IsometricAnimationSet::LoadFromResourceManager(isometric_animation);
    WorldObject* wobj = new WorldObject(duration);
    wobj->AddComponent(new component::Graphic(spritesheet, set, light_radius));
    PrepareProjectile(wobj, dir, speed);
    return wobj;
}

WorldObject* MagicMissile(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICMISSILE_SPEED"), constants::GetDouble("MAGICMISSILE_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));

    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICMISSILE_DAMAGE"));
    wobj->AddComponent(new component::Shape(colobj, NULL));
    return wobj;
}

WorldObject* MagicBall(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICBALL_SPEED"), constants::GetDouble("MAGICBALL_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICBALL_DAMAGE"));
    colobj->AddCollisionLogic("Wall", BounceCollision(wobj));
    wobj->AddComponent(new component::Shape(colobj, NULL));
    return wobj;
}
/***/
WorldObject* MummyProjectile(const ugdk::math::Vector2D &dir, double damage) {
    WorldObject* wobj = buildProjectile(dir, "mummy_projectile", "", 0.75, constants::GetDouble("MUMMYPROJECTILE_SPEED"), constants::GetDouble("MUMMYPROJECTILE_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Hero", DamageAndDieCollision(wobj, damage));
    wobj->AddComponent(new component::Shape(colobj, NULL));
    return wobj;
}

WorldObject* LightningBolt(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "lightning_bolt", "animations/lightning.gdd", 1.0, constants::GetDouble("LIGHTNING_SPEED"), constants::GetDouble("LIGHTNING_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.25);
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));
    wobj->graphic()->ChangeLightColor(ugdk::Color(121/255.0, 229/255.0, 1.0)); // Orange
    wobj->AddComponent(new component::Animation(wobj, utils::IDLE, Direction::FromWorldVector(dir)));
    colobj->AddCollisionLogic("Mummy", DamageCollision("LIGHTNING_DAMAGE"));
    wobj->set_start_to_die_callback(std::mem_fn(&WorldObject::Remove));
    wobj->AddComponent(new component::Shape(colobj, NULL));
    return wobj;
}

WorldObject* Fireball(const Vector2D &dir) {
    WorldObject* wobj = buildProjectile(dir, "fireball", "animations/fireball.gdd", 1.0, constants::GetDouble("FIREBALL_SPEED"), constants::GetDouble("FIREBALL_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.25);
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));
    wobj->graphic()->ChangeLightColor(ugdk::Color(1.0, 0.521568, 0.082352)); // Orange
    wobj->AddComponent(new component::Animation(wobj, utils::IDLE, Direction::FromWorldVector(dir)));
    colobj->AddCollisionLogic("Mummy", DieCollision(wobj));
    wobj->set_start_to_die_callback(std::mem_fn(&WorldObject::Remove));
    wobj->AddDeathEvent(Carrier(builder::ExplosionBuilder::FireballExplosion()));
    wobj->AddComponent(new component::Shape(colobj, NULL));
    return wobj;
}

} // namespace ProjectileBuilder
} // namespace builder
