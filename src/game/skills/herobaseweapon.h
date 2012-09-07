#ifndef HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/components.h"

namespace skills {

class HeroBaseWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroBaseWeapon(component::Caster* owner);
    virtual void Use();

  private:
    typedef CombatArt<usearguments::Aim> super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
