#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_
#include "weapon.h"
#include "game/sprites/hero.h"

namespace sprite {

class Hero;

class HeroBaseWeapon : public Weapon {
  public:
    HeroBaseWeapon(Hero* owner) : Weapon(owner), hero_(owner) {}

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available();

  private:

    Hero *hero_;

};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_ */
