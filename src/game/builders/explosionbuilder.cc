#include "explosionbuilder.h"

#include <functional>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>


#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/logic/follower.h"
#include "game/components/shape.h"
#include "game/scenes/world.h"
#include "game/utils/imagefactory.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {
namespace ExplosionBuilder {

using std::bind;
using ugdk::action::SpriteAnimationTable;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using pyramidworks::collision::CollisionObject;
using component::Follower;
using component::Shape;
using sprite::WorldObject;

COLLISION_DIRECT(double, ExplosionCollision, obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(data_);
}

static WorldObject* baseExplosion(const std::string& spritesheet, const std::string& anim) {
    WorldObject *wobj = new WorldObject;

    utils::IsometricAnimationSet* set = utils::IsometricAnimationSet::LoadFromResourceManager("animations/explosion.gdd");
    component::Graphic* graphic = new component::Graphic(spritesheet, set);
    graphic->ChangeAnimation(anim);
    graphic->AddTickFunction(bind(&WorldObject::StartToDie, wobj));

    wobj->AddComponent(graphic);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Explosion");
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

WorldObject* FireballExplosion() {
    utils::ImageFactory factory;
    WorldObject *wobj = baseExplosion("fireball_explosion", "HERO_FIREBALL_WEAPON");
    
    wobj->component<component::BaseGraphic>()->ChangeLightRadius(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"));
    wobj->component<component::BaseGraphic>()->ChangeLightColor(ugdk::Color(1.0, 0.521568, 0.082352));

    CollisionObject* col = wobj->shape()->collision();
    col->AddCollisionLogic("Mummy", new ExplosionCollision(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE")));
    col->set_shape(new pyramidworks::geometry::Circle(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));

    return wobj;
}

} // namespace ExplosionBuilder
} // namespace builder
