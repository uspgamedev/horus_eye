/*
 * potionbuilder.cc
 *
 *  Created on: 21/08/2010
 *      Author: Henrique
 */

#include "potionbuilder.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

namespace sprite {

using namespace utils;

Potion* PotionBuilder::life_potion() {
    ImageFactory image_factory;
    Potion* potion = new Potion(image_factory.LifePotionImage());
    potion->set_recover_life(Constants::LIFEPOTION_RECOVER_LIFE);
    return potion;
}
Potion* PotionBuilder::mana_potion() {
    ImageFactory image_factory;
    Potion* potion = new Potion(image_factory.ManaPotionImage());
    potion->set_recover_mana(Constants::MANAPOTION_RECOVER_MANA);
    return potion;
}

}
