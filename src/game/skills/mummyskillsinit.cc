#include "game/skills/mummyskills.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>

namespace skills {

Skill* MummyMeleeBuild();
Skill* MummyRangedBuild();
Skill* PaperMeleeBuild();
Skill* PharaohRangedBuild();
Skill* PharaohSummonBuild();

void InitMummySkills() {
    ugdk::base::ResourceContainer<Skill*>& container = RESOURCE_MANAGER()->get_container<skills::Skill*>();
    container.Insert("mummy_melee", MummyMeleeBuild());
    container.Insert("mummy_ranged", MummyRangedBuild());
    container.Insert("paper_melee", PaperMeleeBuild());
    container.Insert("pharaoh_ranged", PharaohRangedBuild());
    container.Insert("pharaoh_summon", PharaohSummonBuild());
}


Skill* MummyMelee() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("mummy_melee");
}
Skill* MummyRanged() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("mummy_ranged");
}
Skill* PaperMelee() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("paper_melee");
}
Skill* PharaohRanged() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("pharaoh_ranged");
}
Skill* PharaohSummon() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("pharaoh_summon");
}

} // skills
