#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYRANGEDWEAPON_H_
#include "weapon.h"
#include "game/utils/constants.h"

namespace sprite {

class MummyRangedWeapon : public Weapon {
  public:
    MummyRangedWeapon(Creature* owner, int damage = 1) :
        Weapon(owner), damage_(damage) {}

    virtual float range(){ return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYRANGEDWEAPON_H_ */
