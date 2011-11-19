
#ifndef HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

class MummyWeapon : public CombatArt<usearguments::Aim> {
  public:
    MummyWeapon(sprite::Creature* owner, int damage = 1)
        : CombatArt<usearguments::Aim>(NULL, 0.0f, owner->mana(), owner->aim()),
          damage_(damage) {}

    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef CombatArt<usearguments::Aim> super;

    int damage_;
    const static float range_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_ */
