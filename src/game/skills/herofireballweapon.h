#ifndef HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_

#include "game/components.h"
#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace skills {

class HeroFireballWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroFireballWeapon(component::Caster* owner);
    virtual void Use();
  private:
    typedef CombatArt<usearguments::Aim> super;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
