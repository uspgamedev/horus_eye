#include <ugdk/time/timeaccumulator.h>

#include "conditionbuilder.h"

#include "game/utils/constants.h"
#include "game/components/logic/creature.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {

using namespace utils;
using component::Creature;
using sprite::Condition;
using sprite::WorldObject;

class IncreaseSightCondition : public Condition {
  public:
    IncreaseSightCondition(WorldObject* owner, double time_condition)
        :   Condition(owner), 
            condition_duration_(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(time_condition))) {}
    ~IncreaseSightCondition() { delete condition_duration_; }

    void Update(double dt);
    void EndCondition(WorldObject *creature);
    void StartCondition(WorldObject *creature);
        
  private:
    ugdk::time::TimeAccumulator *condition_duration_;
};
void IncreaseSightCondition::Update(double dt) {
    if ( phase_ != PHASE_FINISHED && condition_duration_->Expired())
        EndCondition(owner_);
}

void IncreaseSightCondition::StartCondition(WorldObject* obj) {
    Condition::StartCondition(obj);
    obj->set_light_radius(obj->light_radius() + Constants::SIGHT_POTION_INCREASE);
    obj->set_sight_count(1);
}

void IncreaseSightCondition::EndCondition(WorldObject* obj) {
    Condition::EndCondition(obj);
    obj->set_light_radius(obj->light_radius() - Constants::SIGHT_POTION_INCREASE);
    obj->set_sight_count(-1);
}

Condition* ConditionBuilder::increase_sight_condition(WorldObject *owner) {
    return new IncreaseSightCondition(owner, Constants::CONDITION_DURATION);
}

}
