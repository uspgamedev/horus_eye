#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_

#include "game/skills/skill.h"

namespace skills {

/// A skill that has a mana cost.
class CombatArt : public Skill {
  public:
    /**
      @param icon The icon that is displayed on the user interface.
      @param use A function that is called when the skill is used.
      @param mana_cost The mana cost.
      */
    CombatArt(ugdk::graphic::Drawable* icon, SkillUseFunction use, double mana_cost)
      : Skill(icon, use), mana_cost_(mana_cost), range_(-1.0) {}
    
    /**
      @param icon The icon that is displayed on the user interface.
      @param use A function that is called when the skill is used.
      @param valid A function whose return value is considered for IsValidUse
      @param mana_cost The mana cost.
      @param range The maxium range.
      */
    CombatArt(ugdk::graphic::Drawable* icon, SkillUseFunction use, 
              SkillValidFunction valid, double mana_cost, double range)
      : Skill(icon, use, valid), mana_cost_(mana_cost), range_(range) {}

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

    double range() const { return range_; }

  private:
    double mana_cost_, range_;

    typedef Skill super;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
