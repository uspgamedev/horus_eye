#ifndef HORUSEYE_GAME_CONDITION_CONDITION_H_
#define HORUSEYE_GAME_CONDITION_CONDITION_H_

#include "game/sprites.h"

namespace sprite {

class Condition {
  public:
    virtual ~Condition () {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    WorldObject* owner() { return owner_; }
    Phase phase() const { return phase_; }

    virtual void Update(double delta_t) = 0;

    void StartCondition(WorldObject *wobj) {
        owner_ = wobj;
        phase_ = PHASE_ACTIVE;
        onStart();
    }
    
  protected:
  	Condition() : owner_(0), phase_(PHASE_IDLE) {}
  	void EndCondition() { 
        phase_ = PHASE_FINISHED;
        onEnd();
    }

    virtual void onStart() {}
    virtual void onEnd() {}

  private:
  	WorldObject* owner_;
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_CONDITION_CONDITION_H_ */
