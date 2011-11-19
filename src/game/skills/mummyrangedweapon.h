#ifndef HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h" 
#include "game/skills/usearguments.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

class MummyRangedWeapon : public CombatArt<usearguments::Aim> {
  public:
    MummyRangedWeapon(sprite::Creature* owner, int damage = 1) :
        CombatArt<usearguments::Aim>(NULL, 0.0f, owner->mana(), owner->aim()),
        damage_(damage) {}

    virtual float range() const { return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Use();
    virtual bool IsValidUse() const { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_ */
