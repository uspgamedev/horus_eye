#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include "game/components.h"

#include <functional>
#include <string>

namespace skills {

using SkillUseFunction = std::function<void(component::Caster*)>;
using SkillValidFunction = std::function<bool(const component::Caster*)>;

/// An usable skill.
/** Abstract class. Contains an icon.
* @see CombatArt, DivineGift
*/
class Skill {
  public:
    virtual ~Skill() {}

    /// Returns the icon associated with this skill.
    const std::string& icon_path() const {
        return icon_path_;
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
    
    Skill(SkillUseFunction use, SkillValidFunction valid, const std::string& icon_path)
        : use_(use)
        , valid_(valid)
        , icon_path_(icon_path) {}

  private:
    SkillUseFunction use_;
    SkillValidFunction valid_;
    std::string icon_path_;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
