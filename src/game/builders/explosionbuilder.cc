#include "explosionbuilder.h"

#include "game/builders/collision.h"
#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/body.h"
#include "game/components/lightemitter.h"
#include "game/components/animator.h"
#include "game/core/world.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#include <ugdk/system/compatibility.h>
#include <ugdk/action/animationplayer.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/geometry/circle.h>

#include <functional>

namespace builder {
namespace ExplosionBuilder {

using std::bind;
using pyramidworks::collision::CollisionObject;
using component::Body;
using sprite::WorldObject;

static sprite::WObjPtr baseExplosion(const std::string& spritesheet, const std::string& anim) {
    sprite::WObjPtr wobj = WorldObject::Create();
    wobj->set_identifier("Explosion");

    wobj->AddComponent(component::Graphic::CreateWithSpritesheet(spritesheet));

    auto animator = new component::Animator("resources/animations/explosion.json");
    animator->ChangeAnimation(anim);
    animator->AddTickFunction(bind(&WorldObject::Remove, wobj.get()));
    wobj->AddComponent(animator);

    return wobj;
}

sprite::WObjPtr FireballExplosion() {
    sprite::WObjPtr wobj = baseExplosion("effect", "HERO_FIREBALL_WEAPON");
    wobj->graphic()->set_render_offset(-Vector2D(128, 64));

    wobj->AddComponent(new component::LightEmitter(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"), ugdk::structure::Color(1.0, 0.521568, 0.082352)));

    CollisionObject* col = new CollisionObject(nullptr, "Explosion", ugdk::MakeUnique<pyramidworks::geometry::Circle>(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));
    wobj->AddComponent(new Body(col, nullptr));
    col->AddCollisionLogic("Creature", builder::DamageCollision(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE")));

    return wobj;
}

} // namespace ExplosionBuilder
} // namespace builder
