#ifndef HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/castarguments.h"

namespace skills {

class HeroMeteorWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroMeteorWeapon(sprite::Hero* owner);

    virtual float range() const { return 0.0f; }
    virtual void Attack();
    virtual bool Available() const;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_ */
