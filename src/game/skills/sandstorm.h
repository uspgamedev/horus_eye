#ifndef HORUSEYE_GAME_SKILLS_SANDSTORM_H_
#define HORUSEYE_GAME_SKILLS_SANDSTORM_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/sprites/creatures/hero.h"

namespace skills {

class Sandstorm : public CombatArt<usearguments::Aim> {
  public:
    Sandstorm(sprite::Hero* owner);
    virtual void Use();

    virtual bool Available() const {
        return caster_mana_.Has(actual_mana_cost_);
    }

  private:
    const float actual_mana_cost_;
    typedef CombatArt<usearguments::Aim> super;
};

} // namespace skills

#endif
