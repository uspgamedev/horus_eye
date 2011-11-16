
#ifndef HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/castarguments.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

class MummyWeapon : public CombatArt<castarguments::Aim> {
  public:
    MummyWeapon(sprite::Creature* owner, int damage = 1)
        : CombatArt<castarguments::Aim>(NULL, 0.0f, owner->aim()),
          damage_(damage) {}

    virtual float range(){ return 1.0f; }
    virtual void Attack();
    virtual bool Available() const { return true; }
  private:
    int damage_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_ */
