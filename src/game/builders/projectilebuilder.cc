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
#include "game/components/timedlife.h"
#include "game/core/world.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#include <ugdk/resource/module.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/system/engine.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <ugdk/graphic/sprite.h>
#include <cmath>

namespace builder {
namespace ProjectileBuilder {

using ugdk::math::Vector2D;
using pyramidworks::collision::CollisionObject;
using component::Direction;
using utils::IsometricAnimationSet;
using sprite::WorldObject;
using function::Carrier;

static CollisionObject* buildCollisionObject(const sprite::WObjPtr& wobj, double radius) {
    CollisionObject* col = new CollisionObject(wobj.get(), "Projectile", ugdk::MakeUnique<pyramidworks::geometry::Circle>(radius));
    col->AddCollisionLogic("Wall", DieCollision(wobj));
    return col;
}

void PrepareProjectile(const sprite::ObjectHandle& wobj, const ugdk::math::Vector2D &dir, double speed) {
    wobj->AddComponent(new component::StateController(Direction::FromWorldVector(dir), dir));
    wobj->AddComponent(new component::Walker(speed));
}

static sprite::WObjPtr buildProjectile(const ugdk::math::Vector2D &dir,
                                       component::Graphic* graphic, 
                                       component::Animator* animator,
                                       double light_radius, double speed, double duration) {

    sprite::WObjPtr wobj = WorldObject::Create();
    wobj->set_identifier("Projectile");
    wobj->AddComponent(new component::Damageable());
    wobj->AddComponent(graphic);
    if (animator)
        wobj->AddComponent(animator);
    wobj->AddComponent(new component::LightEmitter(light_radius));
    if (duration > 0.0)
        wobj->AddComponent(new component::TimedLife(duration));
    PrepareProjectile(wobj, dir, speed);
    return wobj;
}

sprite::WObjPtr MagicMissile(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir,
                                           component::Graphic::CreateWithSingleFrame("projectile", "blue-ball"),
                                           nullptr,
                                           1.0,
                                           constants::GetDouble("MAGICMISSILE_SPEED"),
                                           constants::GetDouble("MAGICMISSILE_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0) - Vector2D(16.0, 16.0));

    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICMISSILE_DAMAGE"));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr MagicBall(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir,
                                           component::Graphic::CreateWithSingleFrame("projectile", "blue-ball"),
                                           nullptr,
                                           1.0,
                                           constants::GetDouble("MAGICBALL_SPEED"),
                                           constants::GetDouble("MAGICBALL_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0) - Vector2D(16.0, 16.0));

    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Mummy", DamageAndDieCollision(wobj, "MAGICBALL_DAMAGE"));
    colobj->AddCollisionLogic("Wall", BounceCollision(wobj));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}
/***/
sprite::WObjPtr MummyProjectile(const ugdk::math::Vector2D &dir, double damage) {
    sprite::WObjPtr wobj = buildProjectile(dir,
                                           component::Graphic::CreateWithSingleFrame("projectile", "green-ball"),
                                           nullptr,
                                           0.75,
                                           constants::GetDouble("MUMMYPROJECTILE_SPEED"),
                                           constants::GetDouble("MUMMYPROJECTILE_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0) - Vector2D(16.0, 16.0));

    auto colobj = buildCollisionObject(wobj, 0.15);
    colobj->AddCollisionLogic("Hero", DamageAndDieCollision(wobj, damage));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr LightningBolt(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir,
                                           component::Graphic::CreateWithSpritesheet("projectile"),
                                           new component::Animator("resources/animations/lightningbolt.json"),
                                           1.0,
                                           constants::GetDouble("LIGHTNING_SPEED"),
                                           constants::GetDouble("LIGHTNING_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0) - Vector2D(32.0, 32.0));

    wobj->light()->ChangeColor(ugdk::structure::Color(121/255.0, 229/255.0, 1.0)); // Orange
    wobj->AddComponent(new component::Animation(utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->damageable()->AddOnDieCallback(std::mem_fn(&WorldObject::Remove));

    auto colobj = buildCollisionObject(wobj, 0.25);
    colobj->AddCollisionLogic("Mummy", DamageCollision("LIGHTNING_DAMAGE"));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

sprite::WObjPtr Fireball(const Vector2D &dir) {
    sprite::WObjPtr wobj = buildProjectile(dir,
                                           component::Graphic::CreateWithSpritesheet("projectile"),
                                           new component::Animator("resources/animations/fireball.json"),
                                           1.0,
                                           constants::GetDouble("FIREBALL_SPEED"),
                                           constants::GetDouble("FIREBALL_DURATION"));
    wobj->graphic()->set_render_offset(Vector2D(0.0, -58.0) - Vector2D(32.0, 32.0));

    wobj->light()->ChangeColor(ugdk::structure::Color(1.0, 0.521568, 0.082352)); // Orange
    wobj->AddComponent(new component::Animation(utils::IDLE, Direction::FromWorldVector(dir)));
    wobj->damageable()->AddOnDieCallback(std::mem_fn(&WorldObject::Remove));
    wobj->damageable()->AddOnDieCallback(Carrier(builder::ExplosionBuilder::FireballExplosion()));

    auto colobj = buildCollisionObject(wobj, 0.25);
    colobj->AddCollisionLogic("Mummy", DieCollision(wobj));
    wobj->AddComponent(new component::Body(colobj, NULL));
    return wobj;
}

} // namespace ProjectileBuilder
} // namespace builder
