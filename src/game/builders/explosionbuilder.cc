#include "explosionbuilder.h"

#include <functional>
#include <ugdk/action/animationplayer.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/geometry/circle.h>

#include "game/builders/collision.h"
#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/body.h"
#include "game/components/lightemitter.h"
#include "game/components/animator.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

namespace builder {
namespace ExplosionBuilder {

using std::bind;
using pyramidworks::collision::CollisionObject;
using component::Body;
using sprite::WorldObject;

static sprite::WObjPtr baseExplosion(const std::string& spritesheet, const std::string& anim) {
    sprite::WObjPtr wobj = WorldObject::Create();
    wobj->set_identifier("Explosion");

    auto graphic = component::Graphic::Create(spritesheet, "animations/explosion.gdd");
    auto animator = graphic->animator();
    animator->ChangeAnimation(anim);
    animator->AddTickFunction(bind(&WorldObject::Remove, wobj.get()));
    wobj->AddComponent(graphic);

    return wobj;
}

sprite::WObjPtr FireballExplosion() {
    utils::ImageFactory factory;
    sprite::WObjPtr wobj = baseExplosion("fireball_explosion", "HERO_FIREBALL_WEAPON");

    wobj->AddComponent(new component::LightEmitter(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"), ugdk::Color(1.0, 0.521568, 0.082352)));

    CollisionObject* col = new CollisionObject(nullptr, "Explosion", new pyramidworks::geometry::Circle(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));
    wobj->AddComponent(new Body(col, nullptr));
    col->AddCollisionLogic("Creature", builder::DamageCollision(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE")));

    return wobj;
}

} // namespace ExplosionBuilder
} // namespace builder
