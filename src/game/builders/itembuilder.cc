#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>

#include "itembuilder.h"

#include "game/scenes/world.h"
#include "game/utils/constants.h"
#include "game/components/logic/hero.h"
#include "game/components/graphic.h"
#include "game/components/logic/mummy.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/sprites/worldobject.h"
#include "game/sprites/condition.h"
#include "game/sprites/itemevent.h"
#include "game/builders/conditionbuilder.h"
#include "game/builders/entitybuilder.h"

#define INCREASE_SIGHT_TIME 3.00
#define PI 3.141592654
#define PERIOD (2.0*PI/3.0)

namespace ugdk {
class FlexibleSpritesheet;
}

namespace builder {

using namespace utils;
using sprite::WorldObject;
using component::Creature;
using component::Hero;
//using sprite::Follower;
using sprite::Condition;
using pyramidworks::collision::CollisionObject;

struct ItemUseData {
    WorldObject* wobj_;
    sprite::ItemEvent* event_;

    ItemUseData(WorldObject* wobj, sprite::ItemEvent* ev) : wobj_(wobj), event_(ev) {}
};

COLLISION_DIRECT(ItemUseData, UseCollision, obj) {
    WorldObject *wobj = (WorldObject*) obj;
    if (data_.event_->Use(wobj)) {
        data_.wobj_->Die();
        delete data_.event_;
    }
}

UseCollision* CreateItemUse(WorldObject* wobj, sprite::ItemEvent* ev) {
    return new UseCollision(ItemUseData(wobj, ev));
}

class ItemLogic : public component::Logic {
  public:
    ItemLogic(component::Graphic* g, ugdk::graphic::Drawable* image) : total_time_(0) {
        g->node()->AddChild(node_ = new ugdk::graphic::Node(image));
        node_->drawable()->set_hotspot(ugdk::graphic::Drawable::BOTTOM);
    }
    void Update(double delta_t) {
        total_time_ += delta_t;
        if (total_time_ >= PERIOD) total_time_ -= PERIOD;
        node_->modifier()->set_offset(Vector2D(0.0, 10.0*cos(3.0*total_time_)));
    }
  private:
    ugdk::graphic::Node* node_;
    double total_time_;
};

WorldObject* buildBaseItem(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = new WorldObject;
    wobj->set_logic(new ItemLogic(wobj->graphic(), image));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Item");
    col->set_shape(new pyramidworks::geometry::Circle(0.15));

    wobj->set_collision_object(col);
    return wobj;
}

//=======================================
class RecoverLifeEvent : public sprite::ItemEvent {
  public:
    RecoverLifeEvent (int recover) : recover_(recover) {}
    bool Use (sprite::WorldObject *);

  private:
    int recover_;
};

bool RecoverLifeEvent::Use (sprite::WorldObject* hero) {
    resource::Energy& life = hero->damageable()->life();
    if (life < life.max_value()) {
        life += recover_;
        return true;
    }
    return false;
}

//=======================================
class RecoverManaEvent : public sprite::ItemEvent {
    public:
    RecoverManaEvent (int recover) : recover_(recover) {}
    bool Use (sprite::WorldObject* );

    private:
    int recover_;

};

bool RecoverManaEvent::Use (sprite::WorldObject* wobj) {
    Creature* hero = static_cast<Creature*>(wobj->logic());
    if (hero->mana() < hero->max_mana()) {
        hero->set_mana(hero->mana() + recover_);
        return true;
    }
    return false;
}

//=======================================
class IncreaseSightEvent : public sprite::ItemEvent {
    public:
    IncreaseSightEvent (double additional_sight) : additional_sight_(additional_sight) {}
    bool Use (sprite::WorldObject*);

    private:
    double additional_sight_;
    ConditionBuilder condition_builder_;
};

bool IncreaseSightEvent::Use (sprite::WorldObject* wobj) {
    Creature* hero = static_cast<Creature*>(wobj->logic());
    if ( hero->sight_count() < Constants::SIGHT_POTION_MAX_STACK ) {
        Condition* condition = condition_builder_.increase_sight_condition(hero);
        if (hero->AddCondition(condition)) return true;
        else return false;
    }
    else return false;
}

//=======================================

class BlueGemShieldEvent : public sprite::ItemEvent {
  public:
    BlueGemShieldEvent() {}
    bool Use(sprite::WorldObject* hero) {
        EntityBuilder builder;
        WORLD()->AddWorldObject(builder.BlueShieldEntity(hero), hero->world_position());
        return true;
    }
};

//=======================================

WorldObject* ItemBuilder::LifePotion(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = buildBaseItem(image);
    wobj->collision_object()->AddCollisionLogic("Hero", CreateItemUse(wobj, new RecoverLifeEvent(Constants::LIFEPOTION_RECOVER_LIFE)));
    return wobj;
}

WorldObject* ItemBuilder::ManaPotion(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = buildBaseItem(image);
    wobj->collision_object()->AddCollisionLogic("Hero", CreateItemUse(wobj, new RecoverManaEvent(Constants::MANAPOTION_RECOVER_MANA)));
    return wobj;
}

WorldObject* ItemBuilder::SightPotion(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = buildBaseItem(image);
    wobj->collision_object()->AddCollisionLogic("Hero", CreateItemUse(wobj, new IncreaseSightEvent(Constants::SIGHT_POTION_INCREASE)));
    return wobj;
}

WorldObject* ItemBuilder::BlueGem(ugdk::graphic::Drawable* image) {
    WorldObject* wobj = buildBaseItem(image);
    wobj->collision_object()->AddCollisionLogic("Hero", CreateItemUse(wobj, new BlueGemShieldEvent));
    return wobj;
}

}
