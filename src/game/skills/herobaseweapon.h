#ifndef HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"

namespace sprite {

class Hero;

class HeroBaseWeapon : public CombatArt {
  public:
    HeroBaseWeapon(Hero* owner) : CombatArt(owner), hero_(owner) {}

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available();

  private:

    Hero *hero_;

};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
