#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_HEROLIGHTNINGWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_HEROLIGHTNINGWEAPON_H_
#include "game/skills/skill.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"

namespace sprite {

class Hero;

class HeroLightningWeapon : public Weapon {
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

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_ */
