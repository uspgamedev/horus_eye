#include "conditionbuilder.h"

#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"

namespace builder {

using namespace utils;
using sprite::Creature;
using sprite::Condition;

class IncreaseSightCondition : public Condition {
  public:
    IncreaseSightCondition(Creature* owner, float time_condition) : 
      	Condition(owner), condition_duration_(time_condition) {}
    void Update(float dt);
  	void EndCondition(Creature *creature);
  	void StartCondition(Creature *creature);
        
  private:
    float condition_duration_;
};
void IncreaseSightCondition::Update(float dt) {
	if ( phase_ != PHASE_FINISHED ) {
		if (phase_ == PHASE_IDLE) {
			StartCondition(owner_);
			phase_ = PHASE_ACTIVE;
		}
		condition_duration_ -= dt;
		if (condition_duration_ <= 0.0) 
		{
			EndCondition(owner_);
			phase_ = PHASE_FINISHED;
		}
	}
}

void IncreaseSightCondition::StartCondition(Creature* obj) {
	obj->set_light_radius(obj->light_radius() + Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(1);
}

void IncreaseSightCondition::EndCondition(Creature* obj) {
	obj->set_light_radius(obj->light_radius() - Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(-1);
}

Condition* ConditionBuilder::increase_sight_condition(Creature *owner) {
    return new IncreaseSightCondition(owner, Constants::CONDITION_DURATION);
}

}
