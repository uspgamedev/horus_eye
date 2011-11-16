#ifndef HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/skills/castarguments.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

class PharaohRangedWeapon : public CombatArt<castarguments::Aim> {
  public:
    PharaohRangedWeapon(sprite::Creature* owner, int damage = utils::Constants::PHARAOH_RANGED_DAMAGE) :
        CombatArt<castarguments::Aim>(NULL,owner->aim()), damage_(damage) {}

    virtual float range(){ return utils::Constants::RANGED_MUMMY_RANGE; }
    virtual void Attack();
    virtual bool Available() const { return true; }
  private:
    int damage_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHRANGEDWEAPON_H_ */
