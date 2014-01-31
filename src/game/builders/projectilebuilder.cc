#include "projectilebuilder.h"

#include "game/sprites/objecthandle.h"
#include "game/builders/collision.h"
#include "game/builders/explosionbuilder.h"
#include "game/builders/functions/carrier.h"
#include "game/components/animation.h"
#include "game/components/animator.h"
#include "game/components/graphic.h"
#include "game/components/damageable.h"
#include "game/components/walker.h"
#include "game/components/lightemitter.h"
#include "game/components/statecontroller.h"
#include "game/components/body.h"
#include "game/scenes/world.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/structure/types.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <cmath>

namespace builder {
namespace ProjectileBuilder {

using ugdk::graphic::Drawable;
using ugdk::graphic::Sprite;
using ugdk::math::Vector2D;
using pyramidworks::collision::CollisionObject;
using component::Direction;
using utils::IsometricAnimationSet;
using sprite::WorldObject;
using function::Carrier;

static CollisionObject* buildCollisionObject(const sprite::WObjPtr& wobj, double radius) {
    CollisionObject* col = new CollisionObject(wobj.get(), "Projectile", new pyramidworks::geometry::Circle(radius));
    col->AddCollisionLogic("Wall", DieCollision(wobj));
    return col;
}

void PrepareProjectile(const sprite::ObjectHandle& wobj, const ugdk::math::Vector2D &dir, double speed) {
    wobj->AddComponent(new component::StateController(Direction::FromWorldVector(dir), dir));
    wobj->AddComponent(new component::Walker(speed));
}

static sprite::WObjPtr buildProjectile(const ugdk::math::Vector2D &dir, const std::string& spritesheet, const std::string& isometric_animation, 
                                    double light_radius, double speed, double duration) {

    sprite::WObjPtr wobj = WorldObject::Create(duration);
    wobj->set_identifier("Projectile");
    if (isometric_animation.empty())
        wobj->AddComponent(component::Graphic::Create(std::shared_ptr<Drawable>(new Sprite(spritesheet))));
    else
        wobj->AddComponent(component::Graphic::Create(new component::Animator(spritesheet, isometric_animation)));
    wobj->AddComponent(new component::LightEmitter(light_radius));
    PrepareProjectile(wobj, dir, speed);
    return wobj;
}

sprite::WObjPtr MagicMissile(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICMISSILE_SPEED"), constants::GetDouble("MAGICMISSILE_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));

    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICMISSILE_DAMAGE"));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr MagicBall(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir, "magic_missile", "", 1.0, constants::GetDouble("MAGICBALL_SPEED"), constants::GetDouble("MAGICBALL_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICBALL_DAMAGE"));
    colobj->AddCollisionLogic("Wall", BounceCollision(wobj));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}
/***/
sprite::WObjPtr MummyProjectile(const ugdk::math::Vector2D &dir, double damage) {
    sprite::WObjPtr wobj = buildProjectile(dir, "mummy_projectile", "", 0.75, constants::GetDouble("MUMMYPROJECTILE_SPEED"), constants::GetDouble("MUMMYPROJECTILE_DURATION"));
    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Hero", DamageAndDieCollision(wobj, damage));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr LightningBolt(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir, "lightning_bolt", "animations/lightning.gdd", 1.0, constants::GetDouble("LIGHTNING_SPEED"), constants::GetDouble("LIGHTNING_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));
    wobj->light()->ChangeColor(ugdk::Color(121/255.0, 229/255.0, 1.0)); // Orange
    wobj->AddComponent(new component::Animation(utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->set_start_to_die_callback(std::mem_fn(&WorldObject::Remove));
    auto colobj = buildCollisionObject(wobj, 0.25);
    colobj->AddCollisionLogic("Mummy", DamageCollision("LIGHTNING_DAMAGE"));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr Fireball(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir, "fireball", "animations/fireball.gdd", 1.0, constants::GetDouble("FIREBALL_SPEED"), constants::GetDouble("FIREBALL_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0));
    wobj->light()->ChangeColor(ugdk::Color(1.0, 0.521568, 0.082352)); // Orange
    wobj->AddComponent(new component::Animation(utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->set_start_to_die_callback(std::mem_fn(&WorldObject::Remove));
    wobj->AddDeathEvent(Carrier(builder::ExplosionBuilder::FireballExplosion()));
    auto colobj = buildCollisionObject(wobj, 0.25);
    colobj->AddCollisionLogic("Mummy", DieCollision(wobj));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

} // namespace ProjectileBuilder
} // namespace builder
