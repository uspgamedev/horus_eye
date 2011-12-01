#ifndef HORUSEYE_GAME_ENTITIES_SANDSTORMEMITTER_H_
#define HORUSEYE_GAME_ENTITIES_SANDSTORMEMITTER_H_

#include "game/entities/emitterentity.h"

#include "game/utils/constants.h"
#include "game/builders/projectilebuilder.h"

namespace utils {
class Constants;
} // namespace utils

namespace builder {
class ProjectileBuilder;
} // namespace builder

namespace skills {
namespace usearguments {
class Aim;
} // namespace usearguments
} // namespace skills


namespace entities {

using skills::usearguments::Aim;
using utils::Constants;

class SandstormEmitter : public EmitterEntity {
  public:
    SandstormEmitter(const Aim& owners_aim, const builder::ProjectileBuilder& sand_projectile_factory)
      : EmitterEntity(Constants::SANDSTORM_FADEOUT_TIME),
        aim_(owners_aim),
        sand_projectile_factory_(sand_projectile_factory) {}

    ~SandstormEmitter() {}

    // Inherited virtuals
    virtual void Update(float dt);

  protected:
    const Aim& aim_;
    const builder::ProjectileBuilder& sand_projectile_factory_;

  private:
    typedef EmitterEntity super;
};

} // namespace entity

#endif