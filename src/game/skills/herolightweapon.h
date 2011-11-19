#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroLightWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroLightWeapon(sprite::Hero* owner);
    virtual void Use();
    virtual bool IsValidUse() const;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_ */
