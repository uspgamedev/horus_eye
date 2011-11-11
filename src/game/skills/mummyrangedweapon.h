#ifndef HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"

namespace sprite {

class MummyRangedWeapon : public CombatArt {
  public:
    MummyRangedWeapon(Creature* owner, int damage = 1) :
        CombatArt(owner), damage_(damage) {}

    virtual float range(){ return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_ */
