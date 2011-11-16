#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_
#include "game/skills/skill.h"

namespace skills {

template<class CastArgument_T>
class CombatArt : public ArgSkill<CastArgument_T> {
  public:

    virtual ~CombatArt() {}

    typedef CastArgument_T CastArgument;

    // We need to remove these.
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() const = 0;
    // end

    virtual void Use() { Attack(); }
    virtual bool IsValidUse() const { return Available(); }

  protected:
    CombatArt(ugdk::Image* icon, const CastArgument* cast_argument = NULL)
      : ArgSkill<CastArgument>(icon, cast_argument) {}
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
