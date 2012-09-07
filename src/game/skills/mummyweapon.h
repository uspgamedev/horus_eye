
#ifndef HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/components/caster.h"

namespace skills {

class MummyWeapon : public CombatArt<usearguments::Aim> {
  public:
    MummyWeapon(component::Caster* caster, int damage = 1)
        : CombatArt<usearguments::Aim>(NULL, 0.0, caster->mana(), caster->aim()),
          damage_(damage) {}

    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef CombatArt<usearguments::Aim> super;

    int damage_;
    const static double range_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_ */
