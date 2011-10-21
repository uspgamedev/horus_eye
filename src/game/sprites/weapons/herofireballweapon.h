#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_HEROFIREBALLWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_HEROFIREBALLWEAPON_H_

#include "weapon.h"

namespace sprite {

class Hero;

class HeroFireballWeapon : public Weapon {
  public:
    HeroFireballWeapon(Hero* owner);

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available();

  private:

    Hero *hero_;
    int cost_;

};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_ */
