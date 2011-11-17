#ifndef HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"
#include "game/skills/castarguments.h"

namespace skills {

class PharaohSummonWeapon : public CombatArt<castarguments::Aim> {
  public:
    //TODO:FIX 
    PharaohSummonWeapon(sprite::Creature* owner)
        : CombatArt<castarguments::Aim>(NULL, utils::Constants::PHARAOH_SUMMON_MANA_COST,
                                        owner->mana(), owner->aim()),
          owner_(owner) {}
    virtual ~PharaohSummonWeapon() {}

    virtual float range(){ return utils::Constants::PHARAOH_SUMMON_RANGE; }
    virtual void Attack();
    virtual bool Available() const {
        return owner_->mana().Has(cost_);
    }
  protected:
    sprite::Creature* owner_;
};

}

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_ */
