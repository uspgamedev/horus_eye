#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"

#include "game/skills/combatart.h"

namespace ugdk {
class Image;
}

namespace skills {

template<class CastArgument_T>
class DivineGift : public CombatArt<CastArgument_T> {

  public:

    typedef CastArgument_T CastArgument;
    typedef CombatArt<CastArgument> super;

    virtual ~DivineGift() {}

    // We need to remove these.
	virtual float range() const = 0;
	virtual void Attack() = 0;
    //virtual bool Available() const {
    //    return super::caster_mana_.Has(super::mana_cost_) && caster_blocks_.Has(block_cost_);
    //}
    virtual bool Available() const {
        float total_mana_cost = super::mana_cost_ + caster_blocks_.ToMana(block_cost_);
        return super::caster_mana_.Has(total_mana_cost) && caster_blocks_.Has(block_cost_);
    }
    // end

    virtual void Use() { Attack(); }
    virtual bool IsValidUse() const { return Available(); }

  protected:
    DivineGift(ugdk::Image* icon,
               float mana_cost,
               int block_cost,
               resource::Energy& caster_mana,
               resource::CapacityBlocks& caster_blocks,
               const CastArgument& cast_argument
              )
      : CombatArt<CastArgument>(icon, mana_cost, caster_mana, cast_argument),
        block_cost_(block_cost),
        caster_blocks_(caster_blocks) {}

    const int block_cost_;
    resource::CapacityBlocks& caster_blocks_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
