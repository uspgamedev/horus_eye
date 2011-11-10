#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_
#include <ugdk/graphic/image.h>
#include "game/skills/skill.h"

namespace skills {

class CombatArt : public Skill {
  public:
    CombatArt(sprite::Creature* owner) : Skill(owner) {}
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() const = 0;

    virtual void Use() { Attack(); }
    virtual bool IsValidUse() const { return Available(); }
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_COMBATART_H_ */
