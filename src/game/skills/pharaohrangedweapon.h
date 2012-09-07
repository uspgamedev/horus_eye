#ifndef HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/skills/usearguments.h"
#include "game/components/caster.h"

namespace skills {

class PharaohRangedWeapon : public CombatArt<usearguments::Aim> {
  public:
    PharaohRangedWeapon(component::Caster* caster, int damage = utils::Constants::PHARAOH_RANGED_DAMAGE)
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

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_ */
