#ifndef HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h" 
#include "game/skills/castarguments.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

class MummyRangedWeapon : public CombatArt<castarguments::Aim> {
  public:
    MummyRangedWeapon(sprite::Creature* owner, int damage = 1) :
        CombatArt(NULL,owner->aim()), damage_(damage) {}

    virtual float range() { return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() const { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_ */
