#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHRANGEDWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHRANGEDWEAPON_H_
#include "weapon.h"
#include "../../utils/constants.h"

namespace sprite {

class PharaohRangedWeapon : public Weapon {
  public:
    PharaohRangedWeapon(Creature* owner, int damage = utils::Constants::PHARAOH_RANGED_DAMAGE) :
        Weapon(owner), damage_(damage) {}

    virtual float range(){ return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHRANGEDWEAPON_H_ */
