#include "itembuilder.h"
#include "../utils/constants.h"

#define INCREASE_SIGHT_TIME 3.00

namespace framework{
	class Image;
}

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
    if ( hero->sight_count() < Constants::SIGHT_POTION_MAX_STACK ) {
	    Condition* condition = condition_builder_.increase_sight_condition(hero);
	    if (hero->AddCondition(condition)) return true;
	    else return false;
	}
	else return false;
}

Item* ItemBuilder::LifePotion(framework::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverLifeEvent(Constants::LIFEPOTION_RECOVER_LIFE));
    return potion;
}

Item* ItemBuilder::ManaPotion(framework::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverManaEvent(Constants::MANAPOTION_RECOVER_MANA));
    return potion;
}

Item* ItemBuilder::SightPotion(framework::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new IncreaseSightEvent(Constants::SIGHT_POTION_INCREASE));
    return potion;
}

}
