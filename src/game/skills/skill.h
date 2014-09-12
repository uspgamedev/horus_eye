#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include "game/components.h"
#include <ugdk/ui/drawable.h>

#include <functional>
#include <string>
#include <memory>

namespace skills {

using SkillUseFunction = std::function<void(component::Caster*)>;
using SkillValidFunction = std::function<bool(const component::Caster*)>;
using DrawableFactory = std::function<std::unique_ptr<ugdk::ui::Drawable>()>;

/// An usable skill.
/** Abstract class. Contains an icon.
* @see CombatArt, DivineGift
*/
class Skill {
  public:
    virtual ~Skill() {}

    /// Returns the icon associated with this skill.
    std::unique_ptr<ugdk::ui::Drawable> CreateIcon() const {
        return icon_factory_ ? icon_factory_() : nullptr;
    }

    /// Uses the skill.
    virtual void Use(component::Caster* caster) const {
        if(use_) use_(caster);
    }

    /// Verifies if the given caster has the necessary resources to use this skill right now.
    virtual bool Available(const component::Caster*) const = 0;

    /// Verifies if the given caster can use this skill right now.
    virtual bool IsValidUse(const component::Caster* caster) const {
        return !valid_ || valid_(caster);
    }

    static Skill* LoadFromFile(const std::string& filepath) { return nullptr; }

  protected:
    Skill(SkillUseFunction use)
        : use_(use) {}
    
    Skill(SkillUseFunction use, SkillValidFunction valid, DrawableFactory icon_factory)
        : use_(use)
        , valid_(valid)
        , icon_factory_(icon_factory) {}

  private:
    SkillUseFunction use_;
    SkillValidFunction valid_;
    DrawableFactory icon_factory_;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
