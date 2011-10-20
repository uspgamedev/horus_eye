#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_HEROEXPLOSIONWEAPON_H_
#include "weapon.h"
#include "game/sprites/hero.h"
#include "game/utils/constants.h"

namespace sprite {

class Hero;

class HeroExplosionWeapon : public Weapon {
  public:
    HeroExplosionWeapon(Hero* owner);

    virtual float range() { return utils::Constants::QUAKE_EXPLOSION_RANGE; }
    virtual void Attack();
    virtual bool Available();

  private:
    Hero *hero_;
    int cost_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_HEROBASEWEAPON_H_ */
