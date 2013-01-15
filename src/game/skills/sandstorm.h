#ifndef HORUSEYE_GAME_SKILLS_SANDSTORM_H_
#define HORUSEYE_GAME_SKILLS_SANDSTORM_H_

#include "game/utils/constants.h"
#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Creature;
} // namespace sprite

namespace entities {
class SandstormEmitter;
} // namespace entities

namespace skills {

class Sandstorm : public CombatArt<usearguments::Aim> {
  public:
    Sandstorm(sprite::Creature* owner);
    ~Sandstorm();

    // inherited virtuals
    virtual void Use();
    virtual bool Available() const {
        return emitter_
          ? caster_mana_.Has(utils::Constants::SANDSTORM_MAINTAIN_COST)
          : caster_mana_.Has(utils::Constants::SANDSTORM_COST)
        ;
    }
    //virtual bool IsValidUse() const { return true; }

  protected:
    entities::SandstormEmitter* emitter_;
    const double maintain_mana_cost_;

  private:
    typedef CombatArt<usearguments::Aim> super;
};

} // namespace skills

#endif