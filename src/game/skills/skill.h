#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_

#include "game/skills/abstractskill.h"

namespace ugdk {
class Image;
}

namespace skills {

template<class CastArgument_T>
class Skill : public AbstractSkill {
  public:
    typedef CastArgument_T CastArgument;

  protected:
    Skill(ugdk::Image* icon, const CastArgument* cast_argument = NULL)
        : AbstractSkill(icon), cast_argument_(cast_argument) {}

    const CastArgument* cast_argument_;
};

// specialisation for void CastArgument. Forces cast_argument_ to have NULL value.
/*
template<>
class Skill<void> : AbstractSkill {
  public:
    typedef void CastArgument;

  protected:
    Skill(ugdk::Image* icon, const void* cast_argument = NULL)
        : icon_(icon), cast_argument_(NULL) {}

    void* cast_argument_;
};*/

} // skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
