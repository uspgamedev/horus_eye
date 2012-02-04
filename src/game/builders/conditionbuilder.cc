#include <ugdk/time/timeaccumulator.h>

#include "conditionbuilder.h"

#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {

using namespace utils;
using sprite::Creature;
using sprite::Condition;

class IncreaseSightCondition : public Condition {
  public:
    IncreaseSightCondition(Creature* owner, float time_condition) 
        :   Condition(owner), 
            condition_duration_(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(time_condition))) {}
    ~IncreaseSightCondition() { delete condition_duration_; }

    void Update(float dt);
  	void EndCondition(Creature *creature);
  	void StartCondition(Creature *creature);
        
  private:
    ugdk::time::TimeAccumulator *condition_duration_;
};
void IncreaseSightCondition::Update(float dt) {
	if ( phase_ != PHASE_FINISHED && condition_duration_->Expired())
	    EndCondition(owner_);
}

void IncreaseSightCondition::StartCondition(Creature* obj) {
    Condition::StartCondition(obj);
	obj->set_light_radius(obj->light_radius() + Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(1);
}

void IncreaseSightCondition::EndCondition(Creature* obj) {
    Condition::EndCondition(obj);
	obj->set_light_radius(obj->light_radius() - Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(-1);
}

Condition* ConditionBuilder::increase_sight_condition(Creature *owner) {
    return new IncreaseSightCondition(owner, Constants::CONDITION_DURATION);
}

}
