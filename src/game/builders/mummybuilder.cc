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
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/walker.h"
#include "game/components/shape.h"
#include "game/scenes/world.h"
#include "game/map/room.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"
#include "game/resources/energy.h"

namespace builder {
namespace MummyBuilder {

using namespace sprite;
using namespace component;
using component::Animation;
using component::Walker;
using resource::Energy;
using ugdk::math::Vector2D;
using ugdk::action::Entity;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

CollisionLogic AntiStackCollision(Walker* data_) {
    return [data_](const CollisionObject* obj) {
        sprite::WorldObject *wobj = dynamic_cast<sprite::WorldObject *>(obj->owner());
        Vector2D deviation = (data_->owner()->world_position() - wobj->world_position()).Normalize() * 0.9;
        data_->set_offset_direction(deviation);
    };
}

static void MummyRoomAdd(sprite::WorldObject* wobj, map::Room* world) {
    //world->IncreaseNumberOfEnemies();
}

static void MummyDeath(sprite::WorldObject* wobj) {
    int potion = rand() % 100;
    WorldObject* potion_obj = NULL;
    if (potion < 20) {
        std::vector<std::string> blank;
        if(potion < 10)
            potion_obj = builder::ItemBuilder::LifePotion(blank);
        else
            potion_obj = builder::ItemBuilder::ManaPotion(blank);
    }
    if(potion_obj) wobj->current_room()->AddObject(potion_obj, wobj->world_position(), map::POSITION_ABSOLUTE);
    //WORLD()->DecreaseEnemyCount();
}

static WorldObject* build_mummy_wobj(const std::string& spritesheetname, double life, double radius, double speed, bool standing) {
    WorldObject* wobj = new WorldObject;
    PrepareBasicMummy(wobj, spritesheetname, life, radius, speed, standing);
    return wobj;
}

void PrepareBasicMummy(WorldObject* wobj, const std::string& spritesheetname,
                       double life, double radius, double speed, bool standing) {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(
            ugdk::resource::GetSpriteAnimationTableFromFile("animations/creature.gdd"));
    }
    std::string aiscript = "basicmummy";


    wobj->AddComponent(new component::Graphic(spritesheetname, ANIMATIONS));
    wobj->AddComponent(new component::Animation(wobj, utils::SPAWNING, Direction()));

    wobj->AddComponent(new component::Damageable(wobj, 300));
    wobj->damageable()->life() = Energy(life);
    for(int i = 1; i <= 4; ++i) {
        char buffer[255];
        sprintf(buffer, "samples/hit%d.wav", i);
        wobj->damageable()->AddHitSound(buffer);
    }

    wobj->component<Animation>()->AddCallback(utils::DEATH, std::mem_fn(&WorldObject::Remove));

    ai::AI* mummyAI = AIBuilder::AIScript(wobj, aiscript);
    if(mummyAI) {
        mummyAI->set_standing(standing);
        wobj->AddComponent( mummyAI );
    } else {
        fprintf(stderr, "Failed to assign ai '%s' to wobj '%s'\n", aiscript.c_str(), wobj->identifier().c_str());
    }

    resource::Energy mana;
    wobj->AddComponent(new Caster(wobj, mana));

    CollisionObject* col = new CollisionObject(wobj, "Mummy", new pyramidworks::geometry::Circle(radius));
    wobj->AddComponent(new component::Shape(col, NULL));

    Walker* walker = new Walker(wobj, speed);
    wobj->AddComponent(walker);
    col->AddCollisionLogic("Mummy", AntiStackCollision(walker));
    col->AddCollisionLogic("Wall", walker->CreateRectCollision());

    wobj->set_identifier("Mummy");
    wobj->set_start_to_die_callback(MummyDeath);
    wobj->set_room_add_callback(MummyRoomAdd);
}

sprite::WorldObject* StandingMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_basic", constants::GetInt("MUMMY_LIFE"), 
        constants::GetDouble("MUMMY_RADIUS"), constants::GetDouble("MUMMY_SPEED"), true);
    wobj->caster()->power().Set(constants::GetInt("MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject* WalkingMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingRangedMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_ranged", constants::GetInt("RANGED_MUMMY_LIFE"), 
        constants::GetDouble("MUMMY_RADIUS"), constants::GetDouble("MUMMY_SPEED"), true);
    wobj->caster()->power().Set(constants::GetInt("RANGED_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_ranged", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject* WalkingRangedMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingRangedMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingBigMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_big", constants::GetInt("BIG_MUMMY_LIFE"), 
        constants::GetDouble("BIG_MUMMY_RADIUS"), constants::GetDouble("BIG_MUMMY_SPEED"), true);
    // TODO: different GDD
    wobj->damageable()->set_super_armor(true);
    wobj->caster()->power().Set(constants::GetInt("BIG_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject * WalkingBigMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingBigMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WorldObject *StandingPaperMummy(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("mummy_basic", constants::GetInt("PAPER_MUMMY_LIFE"), 
        constants::GetDouble("PAPER_MUMMY_RADIUS"), constants::GetDouble("PAPER_MUMMY_SPEED"),
        true);
    wobj->graphic()->ChangeAlpha(0.5);
    wobj->caster()->power().Set(constants::GetInt("PAPER_MUMMY_DAMAGE"));
    wobj->caster()->LearnAndEquipSkill("paper_melee", Controller::PRIMARY);
    return wobj;
}

sprite::WorldObject *WalkingPaperMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingPaperMummy(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

sprite::WorldObject * StandingPharaoh(const std::vector<std::string>& arguments) {
    WorldObject* wobj = build_mummy_wobj("pharaoh", constants::GetInt("PHARAOH_LIFE"), 
        constants::GetDouble("PHARAOH_RADIUS"), constants::GetDouble("PHARAOH_SPEED"), true);
    wobj->damageable()->set_super_armor(true);

    component::Caster* caster = wobj->caster();
    caster->mana() = Energy(constants::GetInt("PHARAOH_MANA"), constants::GetDouble("PHARAOH_MANA_REGEN")); 
    caster->power().Set(constants::GetInt("PHARAOH_DAMAGE"));
    caster->LearnAndEquipSkill("mummy_melee", Controller::PRIMARY);
    caster->LearnAndEquipSkill("pharaoh_ranged", Controller::SECONDARY);
    caster->LearnAndEquipSkill("pharaoh_summon", Controller::SPECIAL1);
    return wobj;
}

sprite::WorldObject * WalkingPharaoh(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingPharaoh(arguments);
    obj->component<ai::AI>()->set_standing(false);
    return obj;
}

} // namespace MummyBuilder

} // namespace builder
