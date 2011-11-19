#ifndef HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/usearguments.h"

namespace skills {

class HeroMeteorWeapon : public CombatArt<usearguments::Aim> {
  public:
    HeroMeteorWeapon(sprite::Hero* owner);
    virtual void Use();
    virtual bool IsValidUse() const;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_ */
