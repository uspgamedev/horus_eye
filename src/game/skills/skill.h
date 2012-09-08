#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include <cstdlib>

#include <ugdk/graphic.h>
#include "game/components.h"

namespace skills {

typedef void (*SkillUseFunction)(component::Caster*);
typedef bool (*SkillValidFunction)(const component::Caster*);

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
    virtual void Use(component::Caster* caster) {
        use_(caster);
    }

    /// Verifies if the given caster has the necessary resources to use this skill right now.
    virtual bool Available(const component::Caster*) const = 0;

    /// Verifies if the given caster can use this skill right now.
    virtual bool IsValidUse(const component::Caster* caster) const {
        return !valid_ || valid_(caster);
    }

  protected:
    /**
      @param icon The icon that is displayed on the user interface.
      */
    Skill(ugdk::graphic::Drawable* icon, SkillUseFunction use, SkillValidFunction valid = NULL) 
        : icon_(icon), use_(use), valid_(valid) {}

  private:
    ugdk::graphic::Drawable* icon_;
    SkillUseFunction use_;
    SkillValidFunction valid_;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
