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
#include "game/components/shape.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

namespace builder {
namespace ExplosionBuilder {

using std::bind;
using pyramidworks::collision::CollisionObject;
using component::Shape;
using sprite::WorldObject;

static WorldObject* baseExplosion(const std::string& spritesheet, const std::string& anim) {
    WorldObject *wobj = new WorldObject;

    utils::IsometricAnimationSet* set = utils::IsometricAnimationSet::LoadFromResourceManager("animations/explosion.gdd");
    component::Graphic* graphic = new component::Graphic(spritesheet, set);
    graphic->ChangeAnimation(anim);
    graphic->AddTickFunction(bind(&WorldObject::Remove, wobj));

    wobj->AddComponent(graphic);


    return wobj;
}

WorldObject* FireballExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion("fireball_explosion", "HERO_FIREBALL_WEAPON");
    
    wobj->component<component::BaseGraphic>()->ChangeLightRadius(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"));
    wobj->component<component::BaseGraphic>()->ChangeLightColor(ugdk::Color(1.0, 0.521568, 0.082352));

    CollisionObject* col = new CollisionObject(wobj, "Explosion", new pyramidworks::geometry::Circle(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));
    wobj->AddComponent(new Shape(col, NULL));
    col->AddCollisionLogic("Mummy", builder::DamageCollision(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE")));

    return wobj;
}

} // namespace ExplosionBuilder
} // namespace builder
