#ifndef HORUSEYE_GAME_CONDITION_CONDITION_H_
#define HORUSEYE_GAME_CONDITION_CONDITION_H_

#include <cstdlib>
#include "game/utils/constants.h"
#include "game/components.h"

namespace sprite {

class Condition {
  public:
    virtual ~Condition () {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    virtual void Update(double delta_t) = 0;
    virtual Phase phase() { return phase_; }

    virtual void StartCondition(component::Creature *creature) { phase_ = PHASE_ACTIVE; }
  	virtual void EndCondition(component::Creature *creature) { phase_ = PHASE_FINISHED; }
    
  protected:
    component::Creature* owner_;
  	Condition(component::Creature* owner) : owner_(owner), phase_(PHASE_IDLE) {}
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_CONDITION_CONDITION_H_ */
