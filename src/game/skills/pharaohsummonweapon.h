#ifndef HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/components/caster.h"
#include "game/skills/usearguments.h"

namespace skills {

class PharaohSummonWeapon : public CombatArt<usearguments::Aim> {
  public:
    PharaohSummonWeapon(component::Caster* owner);
    virtual void Use();
  private:
    typedef CombatArt<usearguments::Aim> super;
};

}

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_ */
