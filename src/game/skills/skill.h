#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include <cstdlib>

#include "game/skills/skill.h"

namespace ugdk {
class Image;
}

namespace skills {

class Skill {
  public:
    virtual ~Skill() {}

    /// Returns the icon associated with this skill.
    ugdk::Image* icon() const { return icon_; }

    /// Uses the skill.
    /** This is a virtual method, the implementation is free to do anything it chooses.
    */
    virtual void Use() = 0;

    /// Verifies if the skill's arguments are valid for an use right now.
    virtual bool IsValidUse() const = 0;

    /// Verifies if the skill has the necessary resourses to use right now.
    virtual bool Avaiable() const = 0;

  protected:
    Skill(ugdk::Image* icon) : icon_(icon) {}
    ugdk::Image* icon_;
};

/**
 *  Template Class for all typed (argument-wise) skills.
 *  Few things reference this, to use skills you should reference the "Skill" interface above.
 */
template<class UseArgument_T>
class ArgSkill : public Skill {
  public:
    typedef UseArgument_T UseArgument;

  protected:
    ArgSkill(ugdk::Image* icon, const UseArgument& use_argument)
        : Skill(icon), use_argument_(use_argument) {}

    const UseArgument& use_argument_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
