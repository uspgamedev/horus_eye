#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_

#include <game/resources/energy.h>
#include "game/skills/skill.h"

namespace skills {

/// A skill that has an argument and a mana cost.
template<class UseArgument_T>
class CombatArt : public ArgSkill<UseArgument_T> {
  public:
    typedef UseArgument_T UseArgument;

    /// Uses the skill, decrementing the caster's mana.
    /** This Use decrements the caster's mana by the mana cost when called. 
        Remember to call super::Use() when reimplementing. */
    virtual void Use() { caster_mana_ -= mana_cost_; }

    /// Verifies if the caster has enough mana.
    /** @return true if mana is greater than the cost */
	virtual bool Available() const { return caster_mana_.Has(mana_cost_); }

    /// A generic CombatArt has no use restrictions.
    /** @return true */
    virtual bool IsValidUse() const { return true; }

  protected:
    /**
      @param icon The icon that is displayed on the user interface.
      @param mana_cost The mana cost.
      @param caster_mana The Energy from where the mana cost is removed.
      @param use_argument The skill's argument.
      */
    CombatArt(ugdk::Image* icon,
              float mana_cost,
              resource::Energy& caster_mana,
              const UseArgument& use_argument)
      : ArgSkill<UseArgument>(icon, use_argument),
        mana_cost_(mana_cost),
        caster_mana_(caster_mana) {}

    resource::Energy& caster_mana() const { return caster_mana_; }

    const float mana_cost_;
    resource::Energy& caster_mana_;

  private:
    typedef ArgSkill<UseArgument_T> super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
