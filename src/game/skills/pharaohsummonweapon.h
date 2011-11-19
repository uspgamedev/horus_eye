#ifndef HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"
#include "game/skills/usearguments.h"

namespace skills {

class PharaohSummonWeapon : public CombatArt<usearguments::Aim> {
  public:
    //TODO:FIX 
    PharaohSummonWeapon(sprite::Creature* owner)
        : CombatArt<usearguments::Aim>(NULL, utils::Constants::PHARAOH_SUMMON_MANA_COST,
                                        owner->mana(), owner->aim()),
          owner_(owner) {}
    virtual ~PharaohSummonWeapon() {}

    virtual float range() const { return utils::Constants::PHARAOH_SUMMON_RANGE; }
    virtual void Use();
    virtual bool IsValidUse() const {
        return owner_->mana().Has(mana_cost_);
    }
  protected:
    sprite::Creature* owner_;
};

}

#endif /* HORUSEYE_GAME_SKILLS_PHARAOHSUMMONWEAPON_H_ */
