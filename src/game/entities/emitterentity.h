#ifndef HORUSEYE_GAME_ENTITIES_EMITTERENTITY_H_
#define HORUSEYE_GAME_ENTITIES_EMITTERENTITY_H_

#include <ugdk/time/timeaccumulator.h>

#include "game/sprites/timedworldobject.h"

namespace entities {

class EmitterEntity : public sprite::TimedWorldObject {
  public:
    EmitterEntity(double duration) : TimedWorldObject(duration), suspended_(false) {}
    ~EmitterEntity() {}

    // Inherited virtuals
    virtual void Update(double dt) { super::Update(dt); };
    virtual void Die() { super::Die(); }

    // Interface, will eventualy be moved to the Emitter interface.
    void Renew() { timed_life_->Restart(); }
    void Fix()   { timed_life_->  Pause(); }
    void Decay() { timed_life_-> Resume(); }

    void Hold()    { suspended_ =  true; }
    void Release() { suspended_ = false; }

    void Suspend() { timed_life_-> Pause(); suspended_ =  true; }
    void Work()    { timed_life_->Resume(); suspended_ = false; }

  protected:
    bool suspended_;

  private:
    typedef sprite::TimedWorldObject super;
};

} // namespace entity

#endif