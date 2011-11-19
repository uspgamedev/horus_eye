#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroLightningWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroLightningWeapon(sprite::Hero* owner);
    virtual void Use();
  private:
    typedef CombatArt<usearguments::Aim> super;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
