#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include "game/skills/skill.h"

namespace ugdk {
class Image;
}

namespace skills {

class Skill {

  public:
    ugdk::Image* icon() const { return icon_; }

    virtual void Use() = 0; // Not const, since it can affect the weapon itself, i.e. cooldowns.
    virtual bool IsValidUse() const = 0;

    // Legacy hacks, we must remove these when we can.
    virtual float range() = 0;
    virtual void Attack() = 0;
    virtual bool Available() const = 0;

  protected:
    Skill(ugdk::Image* icon) : icon_(icon) {}
    ugdk::Image* icon_;
};

/**
 *  Template Class for all typed (argument-wise) skills.
 *  Few things reference this, to use skills you should reference the "Skill" interface above.
 */
template<class CastArgument_T>
class ArgSkill : public Skill {
  public:
    typedef CastArgument_T CastArgument;

  protected:
    ArgSkill(ugdk::Image* icon, const CastArgument* cast_argument = NULL)
        : Skill(icon), cast_argument_(cast_argument) {}

    const CastArgument* cast_argument_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
