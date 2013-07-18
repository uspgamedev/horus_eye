#include "herobuilder.h"

#include <ugdk/resource/module.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>

#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/playercontroller.h"
#include "game/components/walker.h"
#include "game/components/shape.h"
#include "game/components/condition.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/sprites/worldobject.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#include "game/utils/levelmanager.h"
#include "game/scenes/world.h"

namespace builder {
namespace HeroBuilder {

using pyramidworks::collision::CollisionObject;
using sprite::WorldObject;
using resource::Energy;
using resource::CapacityBlocks;
using component::Animation;
using component::Caster;
using component::Walker;
using component::Shape;
using skills::usearguments::Aim;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

static void HeroDeathEvent(sprite::WorldObject* wobj) {
    WORLD()->SetHero(NULL);
    WORLD()->FinishLevel(utils::LevelManager::FINISH_DIE);
}

COLLISION_DIRECT(component::Walker*, MummySlowCollision, mummy) {
    data_->set_current_speed(data_->current_speed() / 1.19);
}

void SetupCollision(sprite::WorldObject* obj) {
    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), obj);
    col->InitializeCollisionClass("Hero");
    col->set_shape(new pyramidworks::geometry::Circle(0.3));
    col->AddCollisionLogic("Wall", component::CreateWalkerRectCollision(obj->component<Walker>()));
    col->AddCollisionLogic("Mummy", new MummySlowCollision(obj->component<component::Walker>()));

    obj->AddComponent(new Shape(col, NULL));
}

sprite::WorldObject* Kha() {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(
            ugdk::resource::GetSpriteAnimationTableFromFile("animations/creature.gdd"));
    }

    component::PlayerController* player_controller;
    Energy life = Energy(constants::GetDouble("HERO_MAX_LIFE"));
    Energy mana = Energy(constants::GetInt("HERO_MAX_MANA_BLOCKS") * constants::GetDouble("HERO_MANA_PER_BLOCK"),
                         constants::GetDouble("HERO_MANA_REGEN_BASE"),
                         constants::GetInt("HERO_BASE_MANA_REGEN_RATIO"));

    WorldObject* hero_wobj = new WorldObject;
    hero_wobj->AddDeathEvent(HeroDeathEvent);
    hero_wobj->set_identifier("Hero");
    hero_wobj->set_tag("hero");
    hero_wobj->AddComponent(new component::Graphic("hero", ANIMATIONS));
    hero_wobj->AddComponent(new component::Animation(hero_wobj));
    hero_wobj->graphic()->ChangeLightRadius(constants::GetDouble("LIGHT_RADIUS_INITIAL"));
    hero_wobj->AddComponent(player_controller = new component::PlayerController(hero_wobj));

    hero_wobj->AddComponent(new component::Damageable(hero_wobj, 1000, true));
    hero_wobj->damageable()->life() = life;
    hero_wobj->damageable()->set_super_armor(true);
    for(int i = 1; i <= 4; ++i) {
        char buffer[255];
        sprintf(buffer, "samples/hit%d.wav", i);
        hero_wobj->damageable()->AddHitSound(buffer);
    }


    hero_wobj->AddComponent(new component::Condition(hero_wobj));
    hero_wobj->component<Animation>()->AddCallback(utils::DEATH, std::mem_fn(&WorldObject::Die));
    hero_wobj->AddComponent(new Caster(hero_wobj, mana, constants::GetInt("HERO_MAX_MANA_BLOCKS"),
    		Aim(hero_wobj->world_position(), hero_wobj->controller()->aim_destination())));
    
    Walker* walker = new Walker(hero_wobj, constants::GetDouble("HERO_SPEED"));
    hero_wobj->AddComponent(walker);

    component::Caster* caster = hero_wobj->caster();

    caster->mana_blocks().Fill();
    caster->mana().Fill();
 
    caster->LearnAndEquipSkill("magic_missile", component::Controller::PRIMARY);

    int id;

    // No weapons for now.

    // player_controller->AddSkill(id = caster->LearnSkill("fireball"));
    // player_controller->AddSkill(caster->LearnSkill("lightning"));
    // player_controller->AddSkill(caster->LearnSkill("light"));
    // caster->EquipSkill(id, component::Controller::SECONDARY);

    // Add here the other initial weapons of the hero.

    SetupCollision(hero_wobj);

    return hero_wobj;
}

} // namespace HeroBuilder
} // namespace builder
