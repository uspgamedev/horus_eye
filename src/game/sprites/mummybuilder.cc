#include "mummybuilder.h"
#include "weapons/mummyweapon.h"
#include "weapons/mummyrangedweapon.h"
#include "weapons/pharaohrangedweapon.h"
#include "weapons/pharaohsummonweapon.h"
#include "../../framework/image.h"

using namespace sprite;
using namespace utils;

Mummy * MummyBuilder::StandingMummy(framework::Image *image) {
    Mummy* mummy = new Mummy(image);
	mummy->set_life(Constants::MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new MummyWeapon(mummy, Constants::MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	if(image) mummy->set_hotspot(Vector2D(image->frame_size().x / 2.0f, image->frame_size().y*6.0f / 7.0f));

	return mummy;
}

Mummy * MummyBuilder::WalkingMummy(framework::Image *image) {
	Mummy* mummy = StandingMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingRangedMummy(framework::Image *image) {
    Mummy* mummy = new Mummy(image);
	mummy->set_life(Constants::RANGED_MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new MummyRangedWeapon(mummy, Constants::RANGED_MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	if(image) mummy->set_hotspot(Vector2D(image->frame_size().x / 2.0f, image->frame_size().y*6.0f / 7.0f));

	return mummy;
}

Mummy * MummyBuilder::RangedMummy(framework::Image *image) {
	Mummy* mummy = StandingRangedMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Mummy * MummyBuilder::StandingBigMummy(framework::Image *image) {
    Mummy *mummy = new Mummy(image);
    mummy->set_life(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
	mummy->set_weapon(new MummyWeapon(mummy, Constants::BIG_MUMMY_DAMAGE));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
    if(image) mummy->set_hotspot(Vector2D(image->frame_size().x / 2.0f, image->frame_size().y*6.0f / 7.0f));

    return mummy;
}

Mummy * MummyBuilder::BigMummy(framework::Image *image) {
	Mummy* mummy = StandingBigMummy(image);
	mummy->set_standing(false);
	return mummy;
}

Pharaoh * MummyBuilder::StandingPharaoh(framework::Image *image) {
	Pharaoh *pharaoh = new Pharaoh(image, Constants::PHARAOH_LIFE, Constants::PHARAOH_MANA);
    pharaoh->set_speed(Constants::PHARAOH_SPEED);
	pharaoh->set_weapon(new MummyWeapon(pharaoh, Constants::PHARAOH_DAMAGE));
	pharaoh->set_ranged_weapon(new PharaohRangedWeapon(pharaoh, Constants::PHARAOH_RANGED_DAMAGE));
	pharaoh->set_summon_weapon(new PharaohSummonWeapon(pharaoh));
    pharaoh->set_bound(Constants::PHARAOH_RADIUS);
    if(image) pharaoh->set_hotspot(Vector2D(image->frame_size().x / 2.0f, image->frame_size().y*6.0f / 7.0f));

	return pharaoh;
}

Pharaoh * MummyBuilder::WalkingPharaoh(framework::Image *image) {
	Pharaoh* pharaoh = StandingPharaoh(image);
	pharaoh->set_standing(false);
	return pharaoh;
}

