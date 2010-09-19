//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/condition.h
// condition.h da classe Condition
//

#ifndef HORUSEYE_GAME_CONDITION_CONDITION_H_
#define HORUSEYE_GAME_CONDITION_CONDITION_H_

#include <cstdlib>
#include "../utils/constants.h"

namespace sprite {

class Creature;

class Condition {
  public:
    virtual ~Condition () {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    virtual void Update(float delta_t) = 0;
    virtual Phase phase() { return phase_; }
  	virtual void EndCondition(Creature *creature) {}
  	virtual void StartCondition(Creature *creature) {}
    
  protected:
  	Condition (Creature* owner) : owner_(owner), phase_(PHASE_IDLE) {}
  	Creature* owner_;
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_CONDITION_CONDITION_H_ */