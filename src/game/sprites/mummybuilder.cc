#include "mummybuilder.h"
#include "mummyweapon.h"
#include "mummyrangedweapon.h"
#include "../utils/imagefactory.h"

using namespace sprite;
using namespace utils;

Mummy * MummyBuilder::standard_mummy() {
    ImageFactory image_factory;
	Image* mummy_image = image_factory.MummyImage();
    Mummy* mummy = new Mummy(mummy_image);
	mummy->set_life(Constants::MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new MummyWeapon(mummy));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	mummy->set_hotspot(Vector2D(mummy_image->frame_size().x / 2.0, 
								mummy_image->frame_size().y*6.0 / 7.0));

	return mummy;
}

Mummy * MummyBuilder::ranged_mummy() {
    ImageFactory image_factory;
	Image* ranged_mummy_image = image_factory.RangedMummyImage();
    Mummy* mummy = new Mummy(ranged_mummy_image);
	mummy->set_life(Constants::MUMMY_LIFE);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new MummyRangedWeapon(mummy));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	mummy->set_hotspot(Vector2D(ranged_mummy_image->frame_size().x / 2.0, 
								ranged_mummy_image->frame_size().y*6.0 / 7.0));

	return mummy;
}


Mummy * MummyBuilder::big_mummy() {
    ImageFactory image_factory;
	Image* big_mummy_image = image_factory.BigMummyImage();
    Mummy *mummy = new Mummy(big_mummy_image);
    mummy->set_life(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
	mummy->set_weapon(new MummyWeapon(mummy));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
	mummy->set_hotspot(Vector2D(big_mummy_image->frame_size().x / 2.0, 
								big_mummy_image->frame_size().y*6.0 / 7.0));

    return mummy;
}

