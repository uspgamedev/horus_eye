#include "itembuilder.h"

#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/sprites/item.h"
#include "game/sprites/condition.h"
#include "game/builders/conditionbuilder.h"

#define INCREASE_SIGHT_TIME 3.00

namespace ugdk {
class Image;
}

namespace builder {

using namespace utils;
using sprite::Item;
using sprite::Hero;
using sprite::Condition;


//=======================================
class RecoverLifeEvent : public sprite::ItemEvent {
  public:
    RecoverLifeEvent (int recover) : recover_(recover) {}
    bool Use (sprite::Hero *);

  private:
    int recover_;
};

bool RecoverLifeEvent::Use (Hero *hero) {
    if (hero->life() < hero->life().max_value()) {
        hero->life() += recover_;
        return true;
    }
    return false;
}

//=======================================
class RecoverManaEvent : public sprite::ItemEvent {
    public:
    RecoverManaEvent (int recover) : recover_(recover) {}
    bool Use (sprite::Hero *);

    private:
    int recover_;

};

bool RecoverManaEvent::Use (Hero *hero) {
    if (hero->mana() < hero->max_mana()) {
        hero->set_mana(hero->mana() + recover_);
        return true;
    }
    return false;
}

//=======================================
class IncreaseSightEvent : public sprite::ItemEvent {
    public:
    IncreaseSightEvent (float additional_sight) : additional_sight_(additional_sight) {}
    bool Use (sprite::Hero *);

    private:
    float additional_sight_;
    ConditionBuilder condition_builder_;
};

bool IncreaseSightEvent::Use (Hero *hero) {
    if ( hero->sight_count() < Constants::SIGHT_POTION_MAX_STACK ) {
	    Condition* condition = condition_builder_.increase_sight_condition(hero);
	    if (hero->AddCondition(condition)) return true;
	    else return false;
	}
	else return false;
}

//=======================================

Item* ItemBuilder::LifePotion(ugdk::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverLifeEvent(Constants::LIFEPOTION_RECOVER_LIFE));
    return potion;
}

Item* ItemBuilder::ManaPotion(ugdk::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverManaEvent(Constants::MANAPOTION_RECOVER_MANA));
    return potion;
}

Item* ItemBuilder::SightPotion(ugdk::Image* image) {
    Item* potion = new Item(image);
    potion->set_event(new IncreaseSightEvent(Constants::SIGHT_POTION_INCREASE));
    return potion;
}

}
