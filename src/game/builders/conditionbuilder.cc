#include <ugdk/time/timeaccumulator.h>

#include "conditionbuilder.h"

#include "game/utils/constants.h"
#include "game/components/logic/creature.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {

using namespace utils;
using component::Creature;
using sprite::Condition;

class IncreaseSightCondition : public Condition {
  public:
    IncreaseSightCondition(Creature* owner, double time_condition) 
        :   Condition(owner), 
            condition_duration_(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(time_condition))) {}
    ~IncreaseSightCondition() { delete condition_duration_; }

    void Update(double dt);
    void EndCondition(Creature *creature);
    void StartCondition(Creature *creature);
        
  private:
    ugdk::time::TimeAccumulator *condition_duration_;
};
void IncreaseSightCondition::Update(double dt) {
    if ( phase_ != PHASE_FINISHED && condition_duration_->Expired())
        EndCondition(owner_);
}

void IncreaseSightCondition::StartCondition(Creature* obj) {
    Condition::StartCondition(obj);
    obj->owner()->set_light_radius(obj->owner()->light_radius() + Constants::SIGHT_POTION_INCREASE);
    obj->set_sight_count(1);
}

void IncreaseSightCondition::EndCondition(Creature* obj) {
    Condition::EndCondition(obj);
    obj->owner()->set_light_radius(obj->owner()->light_radius() - Constants::SIGHT_POTION_INCREASE);
    obj->set_sight_count(-1);
}

Condition* ConditionBuilder::increase_sight_condition(Creature *owner) {
    return new IncreaseSightCondition(owner, Constants::CONDITION_DURATION);
}

}
