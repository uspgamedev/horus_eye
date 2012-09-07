
#ifndef HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/components/caster.h"

namespace skills {

class PaperMummyWeapon : public CombatArt<usearguments::Aim> {
  public:
    PaperMummyWeapon(component::Caster* caster, int damage = 1)
        : CombatArt<usearguments::Aim>(NULL, 0.0, caster->mana(), caster->aim()),
          damage_(damage),
          owner_(caster) {}

    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef CombatArt<usearguments::Aim> super;

    int damage_;
    const static double range_;
    component::Caster* owner_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_ */
