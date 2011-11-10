#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"

namespace sprite {

class Hero;

class HeroLightningWeapon : public CombatArt {
  public:
    HeroLightningWeapon(Hero* owner);

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available();

  private:

    Hero *hero_;
    int cost_;

};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
