#include "mummybuilder.h"
#include "../utils/imagefactory.h"

using namespace sprite;
using namespace utils;

Mummy * MummyBuilder::standard_mummy() {
    return new Mummy();
}

Mummy * MummyBuilder::big_mummy() {
    ImageFactory image_factory;
    Mummy *mummy = new Mummy(image_factory.BigMummyImage());
    mummy->set_life(Constants::BIG_MUMMY_LIFE);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);

    return mummy;
}
