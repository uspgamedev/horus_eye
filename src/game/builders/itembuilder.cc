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

namespace {

    class ItemLogic : public component::Base {
    public:
        ItemLogic(component::Graphic* g) : graphic_(g), total_time_(0) {}
        void Update(double delta_t) {
            total_time_ += delta_t;
            if (total_time_ >= PERIOD) total_time_ -= PERIOD;
            graphic_->set_render_offset(Vector2D(-13.0, 6.0*cos(3.0*total_time_)-24.0));
        }
    private:
        component::Graphic* graphic_;
        double total_time_;
    };

    
    template <class T>
    sprite::WObjPtr buildBaseItem(T&& ev, const std::string& target_class = "Hero") {
        sprite::WObjPtr wobj = WorldObject::Create();

        wobj->AddComponent(component::Graphic::Create("items"));
        wobj->AddComponent(new ItemLogic(wobj->graphic()), "item", component::orders::LOGIC);

        auto wobj_ptr = wobj.get();
        CollisionObject* col = new CollisionObject(wobj_ptr, "Item", new pyramidworks::geometry::Circle(0.15));
        col->AddCollisionLogic(target_class, [wobj, ev](const CollisionObject* obj) {
            WorldObject *other = dynamic_cast<WorldObject*>(obj->owner());
            if (ev(other))
                wobj->Die();
        });

        wobj->AddComponent(new component::Body(col, nullptr));
        return wobj;
    }

}

//=======================================

WObjPtr LifePotion(const std::vector<std::string>& arguments) {
    int recover_life = constants::GetInt("LIFEPOTION_RECOVER_LIFE");
    WObjPtr wobj = buildBaseItem([recover_life](sprite::WorldObject * wobj) {
        resource::Energy& life = wobj->damageable()->life();
        if (life < life.max_value()) {
            life += recover_life;
            return true;
        }
        return false;
    });
    wobj->graphic()->ChangeToFrame("life_potion");
    return wobj;
}

WObjPtr ManaPotion(const std::vector<std::string>& arguments) {
    int recover_mana = constants::GetInt("MANAPOTION_RECOVER_MANA");
    WObjPtr wobj = buildBaseItem([recover_mana](sprite::WorldObject * wobj) {
        Caster* caster = wobj->caster();
        if (caster && caster->mana() < caster->max_mana()) {
            caster->set_mana(caster->mana() + recover_mana);
            return true;
        }
        return false;
    });
    wobj->graphic()->ChangeToFrame("mana_potion");
    return wobj;
}

} // namespace ItemBuilder
} // namespace builder
