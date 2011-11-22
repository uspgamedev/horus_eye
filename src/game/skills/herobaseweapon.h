#ifndef HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroBaseWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroBaseWeapon(sprite::Hero* owner);
    virtual void Use();

  private:
    typedef CombatArt<usearguments::Aim> super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
