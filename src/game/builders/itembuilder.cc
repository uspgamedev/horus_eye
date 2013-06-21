#include "itembuilder.h"

#include <ugdk/portable/tr1.h>
#include <cmath>
#include FROM_TR1(functional)
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>


#include "game/constants.h"
#include "game/scenes/world.h"
#include "game/map/room.h"
#include "game/components/graphic.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/shape.h"
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
using component::Caster;
using sprite::Effect;
using pyramidworks::collision::CollisionObject;

typedef std::tr1::function<bool (WorldObject*)> ItemEvent;

struct ItemUseData {
    WorldObject* wobj_;
    ItemEvent event_;

    ItemUseData(WorldObject* wobj, const ItemEvent& ev) : wobj_(wobj), event_(ev) {}
};


COLLISION_DIRECT(ItemUseData, UseCollision, obj) {
    WorldObject *wobj = (WorldObject*) obj;
    if (data_.event_(wobj))
        data_.wobj_->Die();
}

void CreateItemUse(WorldObject* wobj, const ItemEvent& ev) {
    wobj->shape()->collision()->AddCollisionLogic("Hero", new UseCollision(ItemUseData(wobj, ev)));
}

class ItemLogic : public component::Base {
  public:
    ItemLogic(component::BaseGraphic* g, ugdk::graphic::Drawable* image) : graphic_(g), total_time_(0) {
    }
    void Update(double delta_t) {
        total_time_ += delta_t;
        if (total_time_ >= PERIOD) total_time_ -= PERIOD;
        graphic_->set_render_offset(Vector2D(0.0, 10.0*cos(3.0*total_time_)));
    }
  private:
    component::BaseGraphic* graphic_;
    double total_time_;
};

WorldObject* buildBaseItem(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic);
    wobj->AddComponent(new ItemLogic(wobj->graphic(), image), "item", component::orders::LOGIC);
    image->set_hotspot(ugdk::graphic::Drawable::BOTTOM);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Item");
    col->set_shape(new pyramidworks::geometry::Circle(0.15));

    wobj->AddComponent(new component::Shape(col, NULL));
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

WorldObject* LifePotion(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = buildBaseItem(factory.LifePotionImage());
    CreateItemUse(wobj, RecoverLifeEvent(constants::GetInt("LIFEPOTION_RECOVER_LIFE")));
    return wobj;
}

WorldObject* ManaPotion(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = buildBaseItem(factory.ManaPotionImage());
    CreateItemUse(wobj, RecoverManaEvent(constants::GetInt("MANAPOTION_RECOVER_MANA")));
    return wobj;
}

} // namespace ItemBuilder
} // namespace builder
