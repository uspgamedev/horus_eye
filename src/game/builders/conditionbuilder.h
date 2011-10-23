#ifndef HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_
#define HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_

#include "game/sprites/condition.h"

namespace sprite {

class Creature;

class ConditionBuilder {
  public:
    ConditionBuilder() {}
    ~ConditionBuilder() {}

    Condition* increase_sight_condition(Creature *owner);

  protected:
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
};

}

#endif /* HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_ */
