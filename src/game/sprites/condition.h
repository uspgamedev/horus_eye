#ifndef HORUSEYE_GAME_CONDITION_CONDITION_H_
#define HORUSEYE_GAME_CONDITION_CONDITION_H_

#include "game/sprites.h"

namespace sprite {

class Condition {
  public:
    virtual ~Condition () {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    virtual void Update(double delta_t) = 0;
    virtual Phase phase() { return phase_; }

    virtual void StartCondition(WorldObject *creature) { phase_ = PHASE_ACTIVE; }
  	virtual void EndCondition(WorldObject *creature) { phase_ = PHASE_FINISHED; }
    
  protected:
  	Condition(WorldObject* owner) : owner_(owner), phase_(PHASE_IDLE) {}

  	WorldObject* owner_;
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_CONDITION_CONDITION_H_ */
