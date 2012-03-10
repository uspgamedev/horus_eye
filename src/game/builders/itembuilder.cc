#include "itembuilder.h"

#include "game/scenes/world.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/creature.h"
//#include "game/sprites/follower.h"
#include "game/sprites/item.h"
#include "game/sprites/condition.h"
#include "game/builders/conditionbuilder.h"
//#include "game/builders/entitybuilder.h"

#define INCREASE_SIGHT_TIME 3.00

namespace ugdk {
class FlexibleSpritesheet;
}

namespace builder {

using namespace utils;
using sprite::Item;
using sprite::Creature;
using sprite::Hero;
//using sprite::Follower;
using sprite::Condition;
using pyramidworks::collision::CollisionObject;


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
    IncreaseSightEvent (double additional_sight) : additional_sight_(additional_sight) {}
    bool Use (sprite::Hero *);

    private:
    double additional_sight_;
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

class BlueGemShieldEvent : public sprite::ItemEvent {
  public:
    BlueGemShieldEvent() {}
    bool Use(sprite::Hero * hero) {
        //EntityBuilder builder;
        //WORLD()->AddWorldObject(builder.BlueShieldEntity(hero), hero->world_position());
        return true;
    }
};

//=======================================

Item* ItemBuilder::LifePotion(ugdk::graphic::FlexibleSpritesheet* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverLifeEvent(Constants::LIFEPOTION_RECOVER_LIFE));
    return potion;
}

Item* ItemBuilder::ManaPotion(ugdk::graphic::FlexibleSpritesheet* image) {
    Item* potion = new Item(image);
    potion->set_event(new RecoverManaEvent(Constants::MANAPOTION_RECOVER_MANA));
    return potion;
}

Item* ItemBuilder::SightPotion(ugdk::graphic::FlexibleSpritesheet* image) {
    Item* potion = new Item(image);
    potion->set_event(new IncreaseSightEvent(Constants::SIGHT_POTION_INCREASE));
    return potion;
}

Item* ItemBuilder::BlueGem(ugdk::graphic::FlexibleSpritesheet* image) {
    Item* gem = new Item(image);
    gem->set_event(new BlueGemShieldEvent);
    return gem;
}

}
