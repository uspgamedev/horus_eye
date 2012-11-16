#include "herobuilder.h"

#include <ugdk/base/resourcemanager.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>

#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/playercontroller.h"
#include "game/components/walker.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/sprites/worldobject.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"

#include "game/utils/levelmanager.h"
#include "game/scenes/world.h"

namespace builder {

using sprite::WorldObject;
using resource::Energy;
using resource::CapacityBlocks;
using component::Animation;
using component::Caster;
using component::Walker;
using skills::usearguments::Aim;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

static void HeroDeathEvent(sprite::WorldObject* wobj) {
    WORLD()->SetHero(NULL);
    WORLD()->FinishLevel(utils::LevelManager::FINISH_DIE);
}

sprite::WorldObject* HeroBuilder::Kha() {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd"));
    }

    component::PlayerController* player_controller;
    Energy life = Energy(constants::GetDouble("HERO_MAX_LIFE"));
    Energy mana = Energy(constants::GetInt("HERO_MAX_MANA_BLOCKS") * constants::GetDouble("HERO_MANA_PER_BLOCK"),
                         constants::GetDouble("HERO_MANA_REGEN_BASE"),
                         constants::GetInt("HERO_BASE_MANA_REGEN_RATIO"));

    WorldObject* hero_wobj = new WorldObject;
    hero_wobj->set_die_callback(HeroDeathEvent);
    hero_wobj->set_identifier("Hero");
    hero_wobj->set_tag("hero");
    hero_wobj->AddComponent(new component::Animation(hero_wobj, "hero", ANIMATIONS));
    hero_wobj->set_light_radius(constants::GetDouble("LIGHT_RADIUS_INITIAL"));
    hero_wobj->AddComponent(player_controller = new component::PlayerController(hero_wobj));
    hero_wobj->AddComponent(new component::Damageable(hero_wobj, 1000, true));
    hero_wobj->damageable()->life() = life;
    hero_wobj->damageable()->set_super_armor(true);
    hero_wobj->component<Animation>()->AddCallback(utils::DEATH, std::tr1::mem_fn(&WorldObject::Die));
    hero_wobj->AddComponent(new Caster(hero_wobj, mana, constants::GetInt("HERO_MAX_MANA_BLOCKS"),
    		Aim(hero_wobj->world_position(), hero_wobj->controller()->aim_destination())));
    
    Walker* walker = new Walker(hero_wobj, constants::GetDouble("HERO_SPEED"));
    hero_wobj->AddComponent(walker);

    component::Caster* caster = hero_wobj->caster();

    caster->mana_blocks().Fill();
    caster->mana().Fill();
 
    caster->LearnAndEquipSkill("magic_missile", component::Controller::PRIMARY);

#ifdef DEBUG
    caster->LearnAndEquipSkill("sandstorm", component::Controller::SPECIAL1);
#endif
    int id;
    player_controller->AddSkill(id = caster->LearnSkill("fireball"));
    player_controller->AddSkill(caster->LearnSkill("quake"));
    player_controller->AddSkill(caster->LearnSkill("lightning"));
    player_controller->AddSkill(caster->LearnSkill("light"));
    player_controller->AddSkill(caster->LearnSkill("meteor"));
    caster->EquipSkill(id, component::Controller::SECONDARY);
    // Add here the other initial weapons of the hero.
    return hero_wobj;
}

COLLISION_DIRECT(component::Walker*, MummySlowCollision, mummy) {
    data_->set_current_speed(data_->current_speed() / 1.19);
}

void HeroBuilder::SetupCollision(sprite::WorldObject* obj) {
    if(obj->collision_object()) {
        delete obj->collision_object();
        obj->set_collision_object(NULL);
    }
    obj->set_collision_object(new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), obj));
    obj->collision_object()->InitializeCollisionClass("Hero");
    obj->collision_object()->set_shape(new pyramidworks::geometry::Circle(0.3));
    obj->collision_object()->AddCollisionLogic("Wall", component::CreateWalkerRectCollision(obj->component<Walker>()));
    obj->collision_object()->AddCollisionLogic("Mummy", new MummySlowCollision(obj->component<component::Walker>()));
}

} // namespace builder
