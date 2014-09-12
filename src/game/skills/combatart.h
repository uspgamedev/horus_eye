#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_

#include "game/skills/skill.h"

namespace skills {

/// A skill that has a mana cost.
class CombatArt : public Skill {
  public:
    /**
    @param use A function that is called when the skill is used.
    @param valid A function whose return value is considered for IsValidUse
    @param icon A functino that creates a new icon for the skill.
    @param mana_cost The mana cost.
    @param range The maxium range.
      */
    CombatArt(SkillUseFunction use,
              SkillValidFunction valid,
              DrawableFactory icon_factory,
              double mana_cost,
              double range = -1.0)
      : Skill(use, valid, icon_factory)
      , mana_cost_(mana_cost)
      , range_(range) {}
    
    /// Uses the skill, decrementing the caster's mana.
    /** This Use decrements the caster's mana by the mana cost when called. */
    virtual void Use(component::Caster*) const;

    /// Verifies if the caster has enough mana.
    /** @return true if mana is greater than the cost */
    virtual bool Available(const component::Caster*) const;

    /// If the range is positive, if the caster target is within range from the caster origin.
    /** @return true if target is within range, or always true if no range is set */
    virtual bool IsValidUse(const component::Caster*) const;

    /// @return The mana cost for this skill.
    double mana_cost() const { return mana_cost_; }

    /// @return The maximum range that this skill can be cast at.
    double range() const { return range_; }

  private:
    double mana_cost_, range_;

    typedef Skill super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
