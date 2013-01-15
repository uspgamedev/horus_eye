#ifndef HORUSEYE_GAME_CONDITION_CONDITION_H_
#define HORUSEYE_GAME_CONDITION_CONDITION_H_

#include <cstdlib>
#include "game/utils/constants.h"

namespace sprite {

class Creature;

class Condition {
  public:
    virtual ~Condition () {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    virtual void Update(double delta_t) = 0;
    virtual Phase phase() { return phase_; }

    virtual void StartCondition(Creature *creature) { phase_ = PHASE_ACTIVE; }
  	virtual void EndCondition(Creature *creature) { phase_ = PHASE_FINISHED; }
    
  protected:
  	Condition (Creature* owner) : owner_(owner), phase_(PHASE_IDLE) {}
  	Creature* owner_;
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_CONDITION_CONDITION_H_ */
