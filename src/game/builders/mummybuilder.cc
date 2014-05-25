#include "mummybuilder.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>

#include "game/builders/itembuilder.h"
#include "game/builders/aibuilder.h"
#include "game/ai/ai.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/animator.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/walker.h"
#include "game/components/body.h"

#include "game/scenes/world.h"
#include "game/map/room.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"
#include "game/resources/energy.h"
#include "game/sprites/objecthandle.h"

#include <unordered_map>

namespace builder {
namespace MummyBuilder {

using namespace sprite;
using namespace component;
using std::string;
using std::unordered_map;
using component::Animation;
using component::Walker;
using resource::Energy;
using ugdk::math::Vector2D;
using ugdk::action::Entity;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;

CollisionLogic AntiStackCollision(Walker* data_) {
    return [data_](const CollisionObject* obj) {
        sprite::WorldObject* wobj = dynamic_cast<sprite::WorldObject*>(obj->owner());
        Vector2D deviation = (data_->owner()->world_position() - wobj->world_position()).Normalize() * 0.9;
        data_->set_offset_direction(deviation);
    };
}

namespace {


void MummyDeath(sprite::WorldObject* wobj) {
    int potion = rand() % 100;
    WObjPtr potion_obj;
    if (potion < 20) {
        std::vector<std::string> blank;
        if(potion < 10)
            potion_obj = builder::ItemBuilder::LifePotion(blank);
        else
            potion_obj = builder::ItemBuilder::ManaPotion(blank);
    }
    if(potion_obj) wobj->current_room()->AddObject(potion_obj, wobj->world_position(), map::POSITION_ABSOLUTE);
}

sprite::WObjPtr build_mummy_wobj(const std::string& spritesheetname,
                                 double life,
                                 double radius,
                                 double speed,
                                 bool standing)
{
    sprite::WObjPtr wobj = WorldObject::Create();
    PrepareBasicMummy(wobj, spritesheetname, life, radius, speed, standing, "resources/animations/creature.json");
    return wobj;
}

}

void PrepareBasicMummy(const sprite::ObjectHandle& wobj,
                       const std::string& spritesheetname,
                       double life,
                       double radius,
                       double speed,
                       bool standing,
                       const string& animation_descriptor) {

    std::string aiscript = "basicmummy";

    wobj->AddComponent(component::Graphic::Create(spritesheetname, animation_descriptor));
    wobj->graphic()->set_render_offset(-Vector2D(60, 120));

    wobj->AddComponent(new component::Animation(utils::IDLE, Direction::Down()));

    wobj->AddComponent(new component::Damageable(300));
    wobj->damageable()->life() = Energy(life);
    for(int i = 1; i <= 4; ++i) {
        char buffer[255];
        sprintf(buffer, "samples/hit%d.wav", i);
        wobj->damageable()->AddHitSound(buffer);
    }

    wobj->component<Animation>()->AddCallback(utils::DEATH, std::mem_fn(&WorldObject::Remove));

    ai::AI* mummyAI = AIBuilder::AIScript(aiscript);
    if(mummyAI) {
        mummyAI->set_standing(standing);
        wobj->AddComponent( mummyAI );
    } else {
        fprintf(stderr, "Failed to assign ai '%s' to wobj '%s'\n", aiscript.c_str(), wobj->identifier().c_str());
    }

    resource::Energy mana;
    wobj->AddComponent(new Caster(wobj, mana));

    Walker* walker = new Walker(speed);
    wobj->AddComponent(walker);

    CollisionObject* col = new CollisionObject(nullptr, "Mummy", new pyramidworks::geometry::Circle(radius));
    wobj->AddComponent(new component::Body(col, nullptr));
    col->AddCollisionLogic("Mummy", AntiStackCollision(walker));
    col->AddCollisionLogic("Wall", walker->CreateRectCollision());

    wobj->set_identifier("Mummy");
    wobj->set_start_to_die_callback(MummyDeath);
}

sprite::WObjPtr StandingMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = build_mummy_wobj("mummy-blue",
                                            constants::GetInt("MUMMY_LIFE"), 
                                            constants::GetDouble("MUMMY_RADIUS"),
                                            constants::GetDouble("MUMMY_SPEED"),
                                            true);
    wobj->caster()->power().Set(constants::GetInt("MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WObjPtr WalkingMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr obj = StandingMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WObjPtr StandingRangedMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = build_mummy_wobj("mummy-red",
                                            constants::GetInt("RANGED_MUMMY_LIFE"),
                                            constants::GetDouble("MUMMY_RADIUS"),
                                            constants::GetDouble("MUMMY_SPEED"),
                                            true);
    wobj->caster()->power().Set(constants::GetInt("RANGED_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_ranged", Controller::PRIMARY);
    return wobj;
}

sprite::WObjPtr WalkingRangedMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr obj = StandingRangedMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WObjPtr StandingBigMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = build_mummy_wobj("mummy-green",
                                            constants::GetInt("BIG_MUMMY_LIFE"),
                                            constants::GetDouble("BIG_MUMMY_RADIUS"),
                                            constants::GetDouble("BIG_MUMMY_SPEED"),
                                            true);
    wobj->damageable()->set_super_armor(true);
    wobj->caster()->power().Set(constants::GetInt("BIG_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WObjPtr WalkingBigMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr obj = StandingBigMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WObjPtr StandingPaperMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = build_mummy_wobj("mummy-blue",
                                            constants::GetInt("PAPER_MUMMY_LIFE"),
                                            constants::GetDouble("PAPER_MUMMY_RADIUS"),
                                            constants::GetDouble("PAPER_MUMMY_SPEED"),
                                            true);
    wobj->graphic()->ChangeAlpha(0.5);
    wobj->caster()->power().Set(constants::GetInt("PAPER_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("paper_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WObjPtr WalkingPaperMummy(const std::vector<std::string>& arguments) {
    sprite::WObjPtr obj = StandingPaperMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WObjPtr StandingPharaoh(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = build_mummy_wobj("pharaoh",
                                            constants::GetInt("PHARAOH_LIFE"),
                                            constants::GetDouble("PHARAOH_RADIUS"),
                                            constants::GetDouble("PHARAOH_SPEED"),
                                            true);
    wobj->damageable()->set_super_armor(true);

    component::Caster* caster = wobj->caster();
    caster->mana() = Energy(constants::GetInt("PHARAOH_MANA"), constants::GetDouble("PHARAOH_MANA_REGEN")); 
    caster->power().Set(constants::GetInt("PHARAOH_DAMAGE"));
    caster->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    caster->LearnAndEquipSkill("pharaoh_ranged", Controller::SECONDARY);
    caster->LearnAndEquipSkill("pharaoh_summon", Controller::SPECIAL1);
    return wobj;
}

sprite::WObjPtr WalkingPharaoh(const std::vector<std::string>& arguments) {
    sprite::WObjPtr obj = StandingPharaoh(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

} // namespace MummyBuilder

} // namespace builder
