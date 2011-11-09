
#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYWEAPON_H_
#include "weapon.h"

namespace sprite {

class MummyWeapon : public Weapon {
  public:
    MummyWeapon(Creature* owner, int damage = 1) : Weapon(owner), damage_(damage) {}

    virtual float range(){ return 1.0f; }
    virtual void Attack();
    virtual bool Available() { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_MUMMYWEAPON_H_ */
