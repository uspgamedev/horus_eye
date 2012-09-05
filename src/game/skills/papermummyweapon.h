
#ifndef HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/components/logic/creature.h"

namespace skills {

class PaperMummyWeapon : public CombatArt<usearguments::Aim> {
  public:
    PaperMummyWeapon(component::Creature* owner, int damage = 1)
        : CombatArt<usearguments::Aim>(NULL, 0.0, owner->owner()->caster()->mana(),
				 owner->owner()->caster()->aim()),
          damage_(damage),
          owner_(owner) {}

    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef CombatArt<usearguments::Aim> super;

    int damage_;
    const static double range_;
    component::Creature* owner_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_PAPERMUMMYWEAPON_H_ */
