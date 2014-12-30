#ifndef HORUSEYE_GAME_EFFECT_H_
#define HORUSEYE_GAME_EFFECT_H_

#include "game/sprites.h"

#include <string>

namespace sprite {

class Effect {
  public:
    virtual ~Effect() {}
    
    enum Phase { PHASE_IDLE, PHASE_ACTIVE, PHASE_FINISHED };
    
    WObjRawPtr owner() { return owner_; }
    Phase phase() const { return phase_; }
    virtual const std::string& name() const = 0;

    virtual void Update(double delta_t) = 0;

    virtual bool CanAffect(WObjRawPtr obj) const = 0;

    void StartEffect(WObjRawPtr wobj) {
        owner_ = wobj;
        phase_ = PHASE_ACTIVE;
        onStart();
    }

  	void EndEffect() { 
        phase_ = PHASE_FINISHED;
        onEnd();
    }
    
  protected:
  	Effect() : owner_(0), phase_(PHASE_IDLE) {}

    virtual void onStart() {}
    virtual void onEnd() {}

  private:
  	WObjRawPtr owner_;
    Phase phase_;
};

}

#endif /* HORUSEYE_GAME_EFFECT_H_ */
