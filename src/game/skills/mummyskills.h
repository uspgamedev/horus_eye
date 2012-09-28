#ifndef HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_
#define HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_

#include "game/skills/combatart.h"

namespace skills {

void InitMummySkills();

Skill* MummyMelee();
Skill* MummyRanged();
Skill* PaperMelee();
Skill* PharaohRanged();
Skill* PharaohSummon();

} // skills

#endif /* HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_ */
