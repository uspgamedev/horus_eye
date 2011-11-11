#ifndef HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/castarguments.h"
#include "game/sprites/creatures/hero.h"

namespace skills {

class HeroBaseWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroBaseWeapon(sprite::Hero* owner) : CombatArt(NULL, owner->aim()) {}

    virtual float range() { return 0.0f; }
    virtual void Attack();
    virtual bool Available() const;

};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
