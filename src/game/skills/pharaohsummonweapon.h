#ifndef HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"
#include "game/skills/usearguments.h"

namespace skills {

class PharaohSummonWeapon : public CombatArt<usearguments::Aim> {
  public:
    PharaohSummonWeapon(sprite::Creature* owner);
    virtual void Use();
  private:
    typedef CombatArt<usearguments::Aim> super;
};

}

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_ */
