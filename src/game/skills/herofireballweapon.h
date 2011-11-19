#ifndef HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroFireballWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroFireballWeapon(sprite::Hero* owner);
    virtual void Use();
  private:
    typedef CombatArt<usearguments::Aim> super;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
