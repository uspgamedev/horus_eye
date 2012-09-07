#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_

#include "game/skills/skill.h"

namespace skills {

/// A skill that has a mana cost.
class CombatArt : public Skill {
  public:
    /**
      @param icon The icon that is displayed on the user interface.
      @param mana_cost The mana cost.
      */
    CombatArt(ugdk::graphic::Drawable* icon, SkillUseFunction use, double mana_cost)
      : Skill(icon), use_(use), mana_cost_(mana_cost) {}

    /// Uses the skill, decrementing the caster's mana.
    /** This Use decrements the caster's mana by the mana cost when called. */
    virtual void Use(component::Caster*);

    /// Verifies if the caster has enough mana.
    /** @return true if mana is greater than the cost */
    virtual bool Available(const component::Caster*) const;

    /// A generic CombatArt has no use restrictions.
    /** @return true */
    virtual bool IsValidUse(const component::Caster*) const;

    /// @return The mana cost for this skill.
    double mana_cost() const { return mana_cost_; }

  private:
    SkillUseFunction use_;
    double mana_cost_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
