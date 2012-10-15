#include "mummybuilder.h"

#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/geometry/circle.h>

#include "game/builders/itembuilder.h"
#include "game/builders/aibuilder.h"
#include "game/ai/ai.h"
#include "game/components/logic/creature.h"
#include "game/components/mummycontroller.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/scenes/world.h"
#include "game/map/room.h"
#include "game/utils/isometricanimationset.h"
#include "game/utils/constants.h"
#include "game/resources/energy.h"

namespace builder {
namespace MummyBuilder {

using namespace sprite;
using namespace component;
using utils::Constants;
using resource::Energy;
using ugdk::Vector2D;
using pyramidworks::collision::CollisionObject;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

COLLISION_DIRECT(Creature*, AntiStackCollision, voiddata) {
    sprite::WorldObject *obj = static_cast<sprite::WorldObject *>(voiddata);
    // TODO: FIX STACKING DECENTLY NOW
    //Vector2D deviation = (data_->owner()->world_position() - obj->world_position()).Normalize();
    //data_->set_direction_vector((data_->direction_vector() + deviation*0.9).Normalize());
}

static void MummyRoomAdd(sprite::WorldObject* wobj, map::Room* world) {
    //world->IncreaseNumberOfEnemies();
}

static void MummyDeath(sprite::WorldObject* wobj) {
    int potion = rand() % 100;
    WorldObject* potion_obj = NULL;
    if (potion <=20){
        std::vector<std::string> blank;
        if(potion > 10)
            potion_obj = builder::ItemBuilder::LifePotion(blank);
        else if(potion> 5)
            potion_obj = builder::ItemBuilder::ManaPotion(blank);
        else
            potion_obj = builder::ItemBuilder::SightPotion(blank);
    }
    if(potion_obj) wobj->current_room()->AddObject(potion_obj, wobj->world_position(), map::POSITION_ABSOLUTE);
    //WORLD()->DecreaseEnemyCount();
}

static WorldObject* build_mummy_wobj(const std::string& tag, double life, double radius, double speed) {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd"));
    }
    WorldObject* wobj = new WorldObject;
    wobj->set_animation(new component::Animation(wobj, tag, ANIMATIONS));
    wobj->set_damageable(new component::Damageable(wobj, 300));
    wobj->damageable()->life() = Energy(life);
    wobj->animation()->AddCallback(utils::DYING, &WorldObject::Die);

    wobj->set_controller( AIBuilder::AIScript(wobj, "basicmummy") );

    resource::Energy mana;
    wobj->set_caster(new Caster(wobj, mana));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Mummy");
    col->set_shape(new pyramidworks::geometry::Circle(radius));
    wobj->set_collision_object(col);

    Creature* mummy = new Creature(wobj, speed);
    wobj->set_logic(mummy);
    col->AddCollisionLogic("Mummy", new AntiStackCollision(mummy));
    col->AddCollisionLogic("Wall", CreateCreatureRectCollision(mummy));

    wobj->set_identifier("Mummy");
    wobj->set_start_to_die_callback(MummyDeath);
    wobj->set_room_add_callback(MummyRoomAdd);
    return wobj;
}

sprite::WorldObject* StandingMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_basic", Constants::MUMMY_LIFE, 
        Constants::MUMMY_RADIUS, Constants::MUMMY_SPEED);

    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject* WalkingMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingMummy(arguments);
    //static_cast<MummyController*>(obj->controller())->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingRangedMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_ranged", Constants::RANGED_MUMMY_LIFE,
        Constants::MUMMY_RADIUS, Constants::MUMMY_SPEED);

    wobj->caster()->LearnAndEquipSkill("mummy_ranged", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject* WalkingRangedMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingRangedMummy(arguments);
    //static_cast<MummyController*>(obj->controller())->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingBigMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_big", Constants::BIG_MUMMY_LIFE,
        Constants::BIG_MUMMY_RADIUS, Constants::BIG_MUMMY_SPEED);
    wobj->node()->modifier()->set_scale(Vector2D(2.0, 2.0));
    wobj->damageable()->set_super_armor(true);

    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    wobj->caster()->power().Set(200);
    return wobj;
}

sprite::WorldObject * WalkingBigMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingBigMummy(arguments);
    //static_cast<MummyController*>(obj->controller())->set_standing(false);
    return obj;
}

sprite::WorldObject *StandingPaperMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_basic", Constants::PAPER_MUMMY_LIFE,
        Constants::MUMMY_RADIUS, Constants::MUMMY_SPEED);
    ugdk::Color color = wobj->graphic()->node()->modifier()->color();
    color.set_a(0.5);
    wobj->graphic()->node()->modifier()->set_color(color);

    wobj->caster()->LearnAndEquipSkill("paper_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject *WalkingPaperMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingPaperMummy(arguments);
    //static_cast<MummyController*>(obj->controller())->set_standing(false);
    return obj;
}

sprite::WorldObject * StandingPharaoh(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("pharaoh", Constants::PHARAOH_LIFE,
        Constants::PHARAOH_RADIUS, Constants::PHARAOH_SPEED);
    wobj->damageable()->set_super_armor(true);

    delete wobj->caster();
    resource::Energy mana(Constants::PHARAOH_MANA, Constants::PHARAOH_MANA_REGEN); 
    wobj->set_caster(new Caster(wobj, mana));

    wobj->caster()->power().Set(300);
    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    wobj->caster()->LearnAndEquipSkill("pharaoh_ranged", Controller::SECONDARY);
    wobj->caster()->LearnAndEquipSkill("pharaoh_summon", Controller::SPECIAL1);
    return wobj;
}

sprite::WorldObject * WalkingPharaoh(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingPharaoh(arguments);
    //static_cast<MummyController*>(obj->controller())->set_standing(false);
    return obj;
}

} // namespace MummyBuilder

} // namespace builder
