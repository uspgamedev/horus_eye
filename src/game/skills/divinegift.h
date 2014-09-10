#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/skills/combatart.h"

namespace skills {

/// A skill that has a mana cost and a block cost.
class DivineGift : public CombatArt {
  public:
    /**
      @param icon The icon that is displayed on the user interface.
      @param use A function that is called when the skill is used.
      @param mana_cost The mana cost.
      @param block_cost The block cost.
      */
    DivineGift(ugdk::ui::Drawable* icon, SkillUseFunction use, double mana_cost, int block_cost)
      : CombatArt(icon, use, mana_cost),
        block_cost_(block_cost) {}
    
    /**
      @param icon The icon that is displayed on the user interface.
      @param use A function that is called when the skill is used.
      @param valid A function whose return value is considered for IsValidUse
      @param mana_cost The mana cost.
      @param block_cost The block cost.
      @param range The maxium range.
      */
    DivineGift(ugdk::ui::Drawable* icon, SkillUseFunction use, SkillValidFunction valid, 
        double mana_cost, int block_cost, double range)
      : CombatArt(icon, use, valid, mana_cost, range),
        block_cost_(block_cost) {}

    /// Uses the skill, decrementing the caster's mana and blocks.
    /** This Use decrements the caster's mana by the mana cost and the caster's
        blocks by the block cost when called. */
    virtual void Use(component::Caster*) const;

    /// Verifies if the caster has enough mana and blocks.
    /** @return mana and blocks are greater than the costs */
    virtual bool Available(const component::Caster*) const;

    /// A generic DivineGift has no use restrictions.
    /** @return true */
    virtual bool IsValidUse(const component::Caster*) const;

    /// @return The block cost of this skill.
    int block_cost() const { return block_cost_; }

  private:
    int block_cost_;

    typedef CombatArt super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
