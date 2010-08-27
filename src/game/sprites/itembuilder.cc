/*
 * potionbuilder.cc
 *
 *  Created on: 21/08/2010
 *      Author: Henrique
 */

#include "itembuilder.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

namespace sprite {

using namespace utils;

bool ItemBuilder::RecoverLifeEvent::Use (Hero *hero) {
    if (hero->life() < hero->max_life()) {
        hero->set_life(hero->life() + recover_);
        return true;
    }
    return false;
}
bool ItemBuilder::RecoverManaEvent::Use (Hero *hero) {
    if (hero->mana() < hero->max_mana()) {
        hero->set_mana(hero->mana() + recover_);
        return true;
    }
    return false;
}
bool ItemBuilder::IncreaseSightEvent::Use (Hero *hero) {
	hero->set_light_radius(hero->light_radius() + additional_sight_);
	return true;
}

Item* ItemBuilder::life_potion() {
    ImageFactory image_factory;
    Item* potion = new Item(image_factory.LifePotionImage());
    potion->set_event(new RecoverLifeEvent(1));
    return potion;
}
Item* ItemBuilder::mana_potion() {
    ImageFactory image_factory;
    Item* potion = new Item(image_factory.ManaPotionImage());
    potion->set_event(new RecoverManaEvent(1));
    return potion;
}
Item* ItemBuilder::sight_potion() {
    ImageFactory image_factory;
    Item* potion = new Item(image_factory.SightPotionImage());
    potion->set_event(new IncreaseSightEvent(1));
    return potion;
}

}
