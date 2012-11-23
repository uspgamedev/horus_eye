#include <ugdk/time/timeaccumulator.h>

#include "conditionbuilder.h"

#include "game/constants.h"
#include "game/sprites/worldobject.h"
#include "game/components/graphic.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {

using sprite::WorldObject;

class IncreaseSightCondition : public sprite::Condition {
  public:
    IncreaseSightCondition(double time_condition)
        :   condition_duration_(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(time_condition))) {}
    ~IncreaseSightCondition() { delete condition_duration_; }

    void Update(double dt) {
        if(condition_duration_->Expired())
            EndCondition();
    }

    void onStart() {
        owner()->graphic()->ChangeLightRadius(owner()->graphic()->light_radius() + constants::GetDouble("SIGHT_POTION_INCREASE"));
    }

    void onEnd() {
        owner()->graphic()->ChangeLightRadius(owner()->graphic()->light_radius() - constants::GetDouble("SIGHT_POTION_INCREASE"));
    }
        
  private:
    ugdk::time::TimeAccumulator *condition_duration_;
};

sprite::Condition* ConditionBuilder::increase_sight_condition() {
    return new IncreaseSightCondition(constants::GetDouble("CONDITION_DURATION"));
}

}
