#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include <cstdlib>

#include <ugdk/graphic.h>
#include "game/components.h"

namespace skills {

typedef void (*SkillUseFunction)(component::Caster*);

/// An usable skill.
/** Abstract class. Contains an icon.
* @see CombatArt, DivineGift
*/
class Skill {
  public:
    virtual ~Skill() {}

    /// Returns the icon associated with this skill.
    ugdk::graphic::Drawable* icon() const { return icon_; }

    /// Uses the skill.
    virtual void Use(component::Caster*) = 0;

    /// Verifies if the skill's arguments are valid for an use right now.
    virtual bool IsValidUse(const component::Caster*) const = 0;

    /// Verifies if the skill has the necessary resources to use right now.
    virtual bool Available(const component::Caster*) const = 0;

  protected:
    /**
      @param icon The icon that is displayed on the user interface.
      */
    Skill(ugdk::graphic::Drawable* icon) : icon_(icon) {}

  private:
    ugdk::graphic::Drawable* icon_;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
