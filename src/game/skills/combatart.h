#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_

#include <game/resources/energy.h>
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

    CombatArt(ugdk::Image* icon,
              float cost,
              resource::Energy& caster_mana,
              const CastArgument& cast_argument)
      : ArgSkill<CastArgument>(icon, cast_argument), cost_(cost), caster_mana_(caster_mana) {}

    const float cost_;
    resource::Energy& caster_mana_;

};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
