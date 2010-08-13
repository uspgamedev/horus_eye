#include "mummybuilder.h"
#include "../utils/imagefactory.h"

using namespace sprite;
using namespace utils;

Mummy * MummyBuilder::standard_mummy() {
    return new Mummy();
}

Mummy * MummyBuilder::big_mummy() {
    ImageFactory image_factory;
	Image* big_mummy_image = image_factory.BigMummyImage();
    Mummy *mummy = new Mummy(big_mummy_image);
    mummy->set_life(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
	mummy->set_hotspot(Vector2D(big_mummy_image->frame_size().x / 2.0, 
								big_mummy_image->frame_size().y*6.0 / 7.0));

    return mummy;
}
