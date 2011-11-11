#include <ugdk/graphic/image.h>

#include "mummybuilder.h"

#include "game/sprites/creatures/mummy.h"
#include "game/sprites/creatures/pharaoh.h"
#include "game/utils/constants.h"
//TODO:REACTIVATE #include "game/skills/mummyweapon.h"
//TODO:REACTIVATE #include "game/skills/mummyrangedweapon.h"
//TODO:REACTIVATE #include "game/skills/pharaohrangedweapon.h"
//TODO:REACTIVATE #include "game/skills/pharaohsummonweapon.h"

namespace builder {

using namespace sprite;
using utils::Constants;

Mummy * MummyBuilder::StandingMummy(ugdk::Image *image) {
    Mummy* mummy = new Mummy(image);
	mummy->set_life(Constants::MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	//TODO:REACTIVATE mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	if(image) mummy->set_hotspot(Vector2D(image->render_size().x / 2.0f, image->render_size().y*6.0f / 7.0f));

	return mummy;
}

Mummy * MummyBuilder::WalkingMummy(ugdk::Image *image) {
	Mummy* mummy = StandingMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingRangedMummy(ugdk::Image *image) {
    Mummy* mummy = new Mummy(image);
	mummy->set_life(Constants::RANGED_MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	//TODO:REACTIVATE mummy->set_weapon(new skills::MummyRangedWeapon(mummy, Constants::RANGED_MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	if(image) mummy->set_hotspot(Vector2D(image->render_size().x / 2.0f, image->render_size().y*6.0f / 7.0f));

	return mummy;
}

Mummy * MummyBuilder::RangedMummy(ugdk::Image *image) {
	Mummy* mummy = StandingRangedMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingBigMummy(ugdk::Image *image) {
    Mummy *mummy = new Mummy(image);
    mummy->set_life(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
	//TODO:REACTIVATE mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::BIG_MUMMY_DAMAGE));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
    mummy->set_super_armor(true);
    if(image) mummy->set_hotspot(Vector2D(image->render_size().x / 2.0f, image->render_size().y*6.0f / 7.0f));

    return mummy;
}

Mummy * MummyBuilder::BigMummy(ugdk::Image *image) {
	Mummy* mummy = StandingBigMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Pharaoh * MummyBuilder::StandingPharaoh(ugdk::Image *image) {
	/*Pharaoh *pharaoh = new Pharaoh(image, Constants::PHARAOH_LIFE, Constants::PHARAOH_MANA);
    pharaoh->set_speed(Constants::PHARAOH_SPEED);
	//TODO:REACTIVATE pharaoh->set_weapon(new skills::MummyWeapon(pharaoh, Constants::PHARAOH_DAMAGE));
	pharaoh->set_ranged_weapon(new skills::PharaohRangedWeapon(pharaoh, Constants::PHARAOH_RANGED_DAMAGE));
	pharaoh->set_summon_weapon(new skills::PharaohSummonWeapon(pharaoh));
    pharaoh->set_bound(Constants::PHARAOH_RADIUS);
    if(image) pharaoh->set_hotspot(Vector2D(image->render_size().x / 2.0f, image->render_size().y*6.0f / 7.0f));
    */
	return NULL; //pharaoh;
}

Pharaoh * MummyBuilder::WalkingPharaoh(ugdk::Image *image) {
	//Pharaoh* pharaoh = StandingPharaoh(image);
	//pharaoh->set_standing(false);
	return NULL; //pharaoh;
}

} // namespace builder