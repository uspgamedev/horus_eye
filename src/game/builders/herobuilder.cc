#include <ugdk/base/resourcemanager.h>

#include "herobuilder.h"

#include "game/components/hero.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/playercontroller.h"
#include "game/sprites/worldobject.h"
#include "game/utils/constants.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/utils/imagefactory.h"
#include "game/utils/isometricanimationset.h"

#include "game/skills/herobaseweapon.h"
#include "game/skills/herofireballweapon.h"
#include "game/skills/heroexplosionweapon.h"
#include "game/skills/herolightningweapon.h"
#include "game/skills/herolightweapon.h"
#include "game/skills/herometeorweapon.h"
#include "game/skills/sandstorm.h"

namespace builder {

using utils::Constants;
using sprite::WorldObject;
using resource::Energy;
using resource::CapacityBlocks;
using component::Hero;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

sprite::WorldObject* HeroBuilder::Kha() {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd"));
    }

    Energy life = Energy(Constants::HERO_MAX_LIFE);
    Energy mana = Energy(Constants::HERO_MAX_MANA_BLOCKS*Constants::HERO_MANA_PER_BLOCK,
                         Constants::HERO_MANA_REGEN_BASE,
                         Constants::HERO_BASE_MANA_REGEN_RATIO);

    WorldObject* hero_wobj = new WorldObject;
    hero_wobj->set_animation(new component::Animation(hero_wobj, factory_->HeroImage(), ANIMATIONS));
    hero_wobj->set_light_radius(Constants::LIGHT_RADIUS_INITIAL);
    hero_wobj->set_controller(new component::PlayerController(hero_wobj));
    hero_wobj->set_damageable(new component::Damageable(hero_wobj, 1000));
    hero_wobj->damageable()->life() = life;
    hero_wobj->damageable()->life().Fill();
    hero_wobj->damageable()->set_super_armor(true);
    hero_wobj->animation()->AddCallback(utils::DYING, &WorldObject::Die);

    Hero *hero = new Hero(hero_wobj, mana, 
                                  Constants::HERO_MAX_MANA_BLOCKS, 
                                  Constants::HERO_MANA_PER_BLOCK);

    hero->mana_blocks().Fill();
    hero->mana().Fill();
    hero->AddWeapon(0, new skills::HeroFireballWeapon(hero));
    hero->AddWeapon(1, new skills::HeroExplosionWeapon(hero));
    hero->AddWeapon(2, new skills::HeroLightningWeapon(hero));
    hero->AddWeapon(3, new skills::HeroLightWeapon(hero));
    hero->AddWeapon(4, new skills::HeroMeteorWeapon(hero));
    hero->AddWeapon(5, new skills::Sandstorm(hero));
    // Add here the other initial weapons of the hero.

    return hero_wobj;
}

} // namespace builder
