#ifndef HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h" 
#include "game/skills/usearguments.h"
#include "game/components/logic/creature.h"

namespace skills {

class MummyRangedWeapon : public CombatArt<usearguments::Aim> {
  public:
    MummyRangedWeapon(component::Creature* owner, int damage = 1) :
        CombatArt<usearguments::Aim>(NULL, 0.0, owner->mana(), owner->aim()),
        damage_(damage) {}

    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef CombatArt<usearguments::Aim> super;

    int damage_;
    const static double range_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_MUMMYRANGEDWEAPON_H_ */
