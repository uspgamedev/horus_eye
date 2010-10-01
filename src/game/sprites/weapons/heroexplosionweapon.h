#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_HEROEXPLOSIONWEAPON_H_
#include "weapon.h"
#include "../hero.h"
#include "../../utils/constants.h"

namespace sprite {

class Hero;

class HeroExplosionWeapon : public Weapon {
  public:
    HeroExplosionWeapon(Hero* owner) : Weapon(owner), hero_(owner), cost_(utils::Constants::QUAKE_COST) {}

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available();

  private:
    Hero *hero_;
    int cost_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_ */
