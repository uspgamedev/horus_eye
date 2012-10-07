#include <ugdk/base/resourcemanager.h>

#include "herobuilder.h"

#include "game/components/logic/hero.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/caster.h"
#include "game/components/playercontroller.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/sprites/worldobject.h"
#include "game/utils/constants.h"
#include "game/utils/isometricanimationset.h"

#include "game/utils/levelmanager.h"
#include "game/scenes/world.h"

namespace builder {

using utils::Constants;
using sprite::WorldObject;
using resource::Energy;
using resource::CapacityBlocks;
using component::Hero;
using component::Caster;
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
    Energy life = Energy(Constants::HERO_MAX_LIFE);
    Energy mana = Energy(Constants::HERO_MAX_MANA_BLOCKS*Constants::HERO_MANA_PER_BLOCK,
                         Constants::HERO_MANA_REGEN_BASE,
                         Constants::HERO_BASE_MANA_REGEN_RATIO);

    WorldObject* hero_wobj = new WorldObject;
    hero_wobj->set_die_callback(HeroDeathEvent);
    hero_wobj->set_identifier("Hero");
    hero_wobj->set_tag("hero");
    hero_wobj->AddComponent(new component::Animation(hero_wobj, "hero", ANIMATIONS));
    hero_wobj->set_light_radius(Constants::LIGHT_RADIUS_INITIAL);
    hero_wobj->AddComponent(player_controller = new component::PlayerController(hero_wobj));
    hero_wobj->AddComponent(new component::Damageable(hero_wobj, 1000, true));
    hero_wobj->damageable()->life() = life;
    hero_wobj->damageable()->set_super_armor(true);
    hero_wobj->animation()->AddCallback(utils::DYING, &WorldObject::Die);
    hero_wobj->AddComponent(new Caster(hero_wobj, mana, Constants::HERO_MAX_MANA_BLOCKS,
    		Aim(hero_wobj->world_position(), hero_wobj->controller()->aim_destination())));
    
    Hero *hero = new Hero(hero_wobj, Constants::HERO_SPEED);
    hero_wobj->AddComponent(hero);

    component::Caster* caster = hero_wobj->caster();

    caster->mana_blocks().Fill();
    caster->mana().Fill();
 
    caster->LearnAndEquipSkill("magic_missile", component::Controller::PRIMARY);

#ifdef DEBUG
    caster->LearnAndEquipSkill("sandstorm", component::Controller::SPECIAL1);
#endif
    /*int id;
    player_controller->AddSkill(id = caster->LearnSkill("fireball"));
    player_controller->AddSkill(caster->LearnSkill("quake"));
    player_controller->AddSkill(caster->LearnSkill("lightning"));
    player_controller->AddSkill(caster->LearnSkill("light"));
    player_controller->AddSkill(caster->LearnSkill("meteor"));
    caster->EquipSkill(id, component::Controller::SECONDARY);
    // Add here the other initial weapons of the hero.
    */
    return hero_wobj;
}

} // namespace builder
