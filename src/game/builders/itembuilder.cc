#include "itembuilder.h"

#include <cmath>
#include <functional>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/primitivesetup.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>

#include "game/constants.h"
#include "game/scenes/world.h"
#include "game/map/room.h"
#include "game/components/graphic.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/body.h"
#include "game/components/condition.h"
#include "game/sprites/worldobject.h"
#include "game/sprites/effect.h"
#include "game/utils/imagefactory.h"

#define INCREASE_SIGHT_TIME 3.00
#define PI 3.141592654
#define PERIOD (2.0*PI/3.0)

namespace builder {
namespace ItemBuilder {

using namespace utils;
using sprite::WorldObject;
using sprite::WObjPtr;
using component::Caster;
using sprite::Effect;
using ugdk::action::Entity;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;

typedef std::function<bool (WorldObject*)> ItemEvent;

struct ItemUseData {
    WorldObject* wobj_;
    ItemEvent event_;

    ItemUseData(WorldObject* wobj, const ItemEvent& ev) : wobj_(wobj), event_(ev) {}
};

CollisionLogic UseCollision(WorldObject* owner, ItemEvent event) {
    return [owner, event](const CollisionObject* obj) {
        WorldObject *wobj = dynamic_cast<WorldObject*>(obj->owner());
        if (event(wobj))
            owner->Die();
    };
}

class ItemLogic : public component::Base {
  public:
    ItemLogic(component::Graphic* g) : graphic_(g), total_time_(0) {
    }
    void Update(double delta_t) {
        total_time_ += delta_t;
        if (total_time_ >= PERIOD) total_time_ -= PERIOD;
        graphic_->set_render_offset(Vector2D(0.0, 10.0*cos(3.0*total_time_)));
    }
  private:
    component::Graphic* graphic_;
    double total_time_;
};

sprite::WObjPtr buildBaseItem(ugdk::internal::GLTexture* texture, const ItemEvent& ev, const std::string& target_class = "Hero") {
    sprite::WObjPtr wobj = WorldObject::Create();

    wobj->AddComponent(component::Graphic::Create([texture](ugdk::graphic::Primitive& p) {
        ugdk::graphic::PrimitiveSetup::Rectangle::Prepare(p, texture);
    }));
    wobj->AddComponent(new ItemLogic(wobj->graphic()), "item", component::orders::LOGIC);

    CollisionObject* col = new CollisionObject(wobj.get(), "Item", new pyramidworks::geometry::Circle(0.15));
    col->AddCollisionLogic(target_class, UseCollision(wobj.get(), ev));

    wobj->AddComponent(new component::Body(col, NULL));
    return wobj;
}

//=======================================
class RecoverLifeEvent {
  public:
    RecoverLifeEvent (int recover) : recover_(recover) {}

    bool operator() (sprite::WorldObject * wobj) {
        resource::Energy& life = wobj->damageable()->life();
        if (life < life.max_value()) {
            life += recover_;
            return true;
        }
        return false;
    }

  private:
    int recover_;
};

//=======================================
class RecoverManaEvent {
  public:
    RecoverManaEvent (int recover) : recover_(recover) {}

    bool operator() (sprite::WorldObject * wobj) {
        Caster* caster = wobj->caster();
        if (caster && caster->mana() < caster->max_mana()) {
            caster->set_mana(caster->mana() + recover_);
            return true;
        }
        return false;
    }

  private:
    int recover_;
};

//=======================================

WObjPtr LifePotion(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WObjPtr wobj = buildBaseItem(factory.LifePotionImage(), RecoverLifeEvent(constants::GetInt("LIFEPOTION_RECOVER_LIFE")));
    return wobj;
}

WObjPtr ManaPotion(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WObjPtr wobj = buildBaseItem(factory.ManaPotionImage(), RecoverManaEvent(constants::GetInt("MANAPOTION_RECOVER_MANA")));
    return wobj;
}

} // namespace ItemBuilder
} // namespace builder
