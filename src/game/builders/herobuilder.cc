

#include "herobuilder.h"

#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/utils/imagefactory.h"

#include "game/skills/herobaseweapon.h"
#include "game/skills/herofireballweapon.h"
#include "game/skills/heroexplosionweapon.h"
#include "game/skills/herolightningweapon.h"
#include "game/skills/herolightweapon.h"
#include "game/skills/herometeorweapon.h"
#include "game/skills/sandstorm.h"

namespace builder {

using namespace sprite;
using utils::Constants;
using resource::Energy;
using resource::CapacityBlocks;

Hero* HeroBuilder::Kha() {
    Energy life = Energy(Constants::HERO_MAX_LIFE);
    Energy mana = Energy(Constants::HERO_MAX_MANA_BLOCKS*Constants::HERO_MANA_PER_BLOCK,
                         Constants::HERO_MANA_REGEN_BASE,
                         Constants::HERO_BASE_MANA_REGEN_RATIO);

    Hero *hero = new sprite::Hero(factory_->HeroImage(), 
                                  life, 
                                  mana, 
                                  Constants::HERO_MAX_MANA_BLOCKS, 
                                  Constants::HERO_MANA_PER_BLOCK);

    hero->set_light_radius(Constants::LIGHT_RADIUS_INITIAL);

	hero->life().Fill();
    hero->mana_blocks().Fill();
    hero->mana().Fill();
	hero->AddWeapon(0, new skills::HeroFireballWeapon(hero));
	hero->AddWeapon(1, new skills::HeroExplosionWeapon(hero));
    hero->AddWeapon(2, new skills::HeroLightningWeapon(hero));
	hero->AddWeapon(3, new skills::HeroLightWeapon(hero));
    hero->AddWeapon(4, new skills::HeroMeteorWeapon(hero));
    hero->AddWeapon(5, new skills::Sandstorm(hero));
    // Add here the other initial weapons of the hero.

	return hero;
}

} // namespace builder
