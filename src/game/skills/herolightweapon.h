#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_

#include "game/components.h"
#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace skills {

class HeroLightWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroLightWeapon(component::Caster* owner);
    virtual void Use();
    virtual bool IsValidUse() const;
  private:
    typedef CombatArt<usearguments::Aim> super;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_ */
