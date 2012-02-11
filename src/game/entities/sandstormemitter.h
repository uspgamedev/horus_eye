#ifndef HORUSEYE_GAME_ENTITIES_SANDSTORMEMITTER_H_
#define HORUSEYE_GAME_ENTITIES_SANDSTORMEMITTER_H_

#include <ugdk/time/timeaccumulator.h>
#include "game/entities/emitterentity.h"
#include "game/utils/constants.h"

#define FLT_SECS_TO_INT_MILLISECS(value) ((int)((value)*1000))

namespace skills {
namespace usearguments {
class Aim;
} // namespace usearguments
} // namespace skills


namespace entities {

class SandstormEmitter : public EmitterEntity {
  public:
    SandstormEmitter(const skills::usearguments::Aim& owners_aim, SandstormEmitter** back_reference)
      : EmitterEntity(utils::Constants::SANDSTORM_FADEOUT_TIME),
        aim_(owners_aim),
        back_reference_(back_reference) {
        projectile_interval_ = new ugdk::time::TimeAccumulator(
            FLT_SECS_TO_INT_MILLISECS(utils::Constants::SANDSTORM_PROJECTILE_INTERVAL)
        );
    }

    ~SandstormEmitter() {
        delete projectile_interval_;
    }

    // Inherited virtuals
    virtual void Update(double dt);
    virtual void Die() { *back_reference_ = NULL; super::Die(); }

  protected:
    const skills::usearguments::Aim& aim_;
    ugdk::time::TimeAccumulator* projectile_interval_;

    SandstormEmitter** back_reference_;

  private:
    typedef EmitterEntity super;
};

} // namespace entity

#endif