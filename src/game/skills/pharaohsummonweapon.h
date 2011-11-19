#ifndef HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"
#include "game/skills/usearguments.h"

namespace skills {

class PharaohSummonWeapon : public CombatArt<usearguments::Aim> {
  private:
    typedef CombatArt<usearguments::Aim> super;
  public:
    PharaohSummonWeapon(sprite::Creature* owner);
    virtual void Use();
};

}

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_ */
