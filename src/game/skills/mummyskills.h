#ifndef HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_
#define HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_

#include "game/skills/combatart.h"

namespace skills {

Skill* MummyMelee(double damage);
Skill* MummyRanged(double damage);
Skill* PaperMelee(double damage);
Skill* PharaohRanged();
Skill* PharaohSummon();

} // skills

#endif /* HORUSEYE_GAME_SKILLS_MUMMYSKILLS_H_ */
