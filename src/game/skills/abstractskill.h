#ifndef HORUSEYE_GAME_SKILLS_ABSTRACTSKILL_H_
#define HORUSEYE_GAME_SKILLS_ABSTRACTSKILL_H_

#include <ugdk/graphic/image.h>

namespace skills {

class AbstractSkill {

  public:
    ugdk::Image* icon() const { return icon_; }

    virtual void Use() = 0; // Not const, since it can affect the weapon itself, i.e. cooldowns.
    virtual bool IsValidUse() const = 0;

    // Legacy hacks, we must remove these when we can.
    virtual float range() = 0;
    virtual void Attack() = 0;
    virtual bool Available() const = 0;

  protected:
    AbstractSkill(ugdk::Image* icon) : icon_(icon) {}
    ugdk::Image* icon_;
};

} // skills

#endif