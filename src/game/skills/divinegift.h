#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/skills/combatart.h"

namespace skills {

/// A skill that has an argument, a mana cost and a block cost.
template<class UseArgument_T>
class DivineGift : public CombatArt<UseArgument_T> {

  public:
    typedef UseArgument_T UseArgument;

    /// Uses the skill, decrementing the caster's mana and blocks.
    /** This Use decrements the caster's mana by the mana cost and the caster's
        blocks by the block cost when called. 
        Remember to call super::Use() when reimplementing. */
    virtual void Use() { super::Use(); caster_blocks_ -= block_cost_; };

    /// Verifies if the caster has enough mana and blocks.
    /** @return mana and blocks are greater than the costs */
    virtual bool Available() const {
        float total_mana_cost = super::mana_cost_ + caster_blocks_.ToMana(block_cost_);
        return super::caster_mana_.Has(total_mana_cost) && caster_blocks_.Has(block_cost_);
    }

    /// A generic DivineGift has no use restrictions.
    /** @return true */
    virtual bool IsValidUse() const { return true; }

  protected:
    /**
      @param icon The icon that is displayed on the user interface.
      @param mana_cost The mana cost.
      @param block_cost The block cost.
      @param caster_mana The Energy from where the mana cost is removed.
      @param caster_blocks The CapacityBlocks from where the block cost is removed.
      @param use_argument The skill's argument.
      */
    DivineGift(ugdk::Drawable* icon,
               float mana_cost,
               int block_cost,
               resource::Energy& caster_mana,
               resource::CapacityBlocks& caster_blocks,
               const UseArgument& use_argument
              )
      : CombatArt<UseArgument>(icon, mana_cost, caster_mana, use_argument),
        block_cost_(block_cost),
        caster_blocks_(caster_blocks) {}

    const int block_cost_;
    resource::CapacityBlocks& caster_blocks_;

  private:
    typedef CombatArt<UseArgument_T> super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
