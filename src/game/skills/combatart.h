#ifndef HORUSEYE_GAME_SKILLS_COMBATART_H_
#define HORUSEYE_GAME_SKILLS_COMBATART_H_
#include <ugdk/graphic/image.h>
#include "game/skills/skill.h"

namespace sprite {
class Creature;
class CombatArt : public Skill {
  public:
    CombatArt(Creature* owner) : Skill(owner) {}
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() = 0;

    void Use() { Attack(); }
    bool IsValidUse() { return Available(); }
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_WEAPON_H_ */
