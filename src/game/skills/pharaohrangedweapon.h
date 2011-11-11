#ifndef HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_
#include "game/skills/combatart.h"
#include "game/utils/constants.h"

namespace sprite {

class PharaohRangedWeapon : public CombatArt {
  public:
    PharaohRangedWeapon(Creature* owner, int damage = utils::Constants::PHARAOH_RANGED_DAMAGE) :
        CombatArt(owner), damage_(damage) {}

    virtual float range(){ return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_ */
