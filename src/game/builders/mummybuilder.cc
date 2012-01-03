#include <ugdk/graphic/drawable/flexiblespritesheet.h>

#include "mummybuilder.h"

#include "game/sprites/creatures/mummy.h"
#include "game/sprites/creatures/pharaoh.h"
#include "game/utils/constants.h"
#include "game/skills/mummyweapon.h"
#include "game/skills/mummyrangedweapon.h"
#include "game/skills/pharaohrangedweapon.h"
#include "game/skills/pharaohsummonweapon.h"
#include <game/resources/energy.h>

namespace builder {

using namespace sprite;
using utils::Constants;
using resource::Energy;

Mummy * MummyBuilder::StandingMummy(ugdk::FlexibleSpritesheet *image) {
    Mummy* mummy = new Mummy(image);
	mummy->life() = Energy(Constants::MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	return mummy;
}

Mummy * MummyBuilder::WalkingMummy(ugdk::FlexibleSpritesheet *image) {
	Mummy* mummy = StandingMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingRangedMummy(ugdk::FlexibleSpritesheet *image) {
    Mummy* mummy = new Mummy(image);
    mummy->life() = Energy(Constants::RANGED_MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyRangedWeapon(mummy, Constants::RANGED_MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	return mummy;
}

Mummy * MummyBuilder::RangedMummy(ugdk::FlexibleSpritesheet *image) {
	Mummy* mummy = StandingRangedMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingBigMummy(ugdk::FlexibleSpritesheet *image) {
    Mummy *mummy = new Mummy(image);
    mummy->life() = Energy(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::BIG_MUMMY_DAMAGE));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
    mummy->set_super_armor(true);
    return mummy;
}

Mummy * MummyBuilder::BigMummy(ugdk::FlexibleSpritesheet *image) {
	Mummy* mummy = StandingBigMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Pharaoh * MummyBuilder::StandingPharaoh(ugdk::FlexibleSpritesheet *image) {
	Pharaoh *pharaoh = new Pharaoh(image, Constants::PHARAOH_LIFE, Constants::PHARAOH_MANA);
    pharaoh->set_speed(Constants::PHARAOH_SPEED);
	pharaoh->set_weapon(new skills::MummyWeapon(pharaoh, Constants::PHARAOH_DAMAGE));
	pharaoh->set_ranged_weapon(new skills::PharaohRangedWeapon(pharaoh, Constants::PHARAOH_RANGED_DAMAGE));
	pharaoh->set_summon_weapon(new skills::PharaohSummonWeapon(pharaoh));
    pharaoh->set_bound(Constants::PHARAOH_RADIUS);
	return pharaoh;
}

Pharaoh * MummyBuilder::WalkingPharaoh(ugdk::FlexibleSpritesheet *image) {
	Pharaoh* pharaoh = StandingPharaoh(image);
	pharaoh->set_standing(false);
	return pharaoh;
}

} // namespace builder
