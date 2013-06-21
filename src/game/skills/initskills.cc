#include "game/skills/initskills.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>

#include "game/skills.h"

namespace skills {

Skill* HeroMagicMissileBuild();
Skill* HeroFireballBuild();
Skill* HeroLightningBuild();
Skill* HeroLightBuild();
Skill* MummyMeleeBuild();
Skill* MummyRangedBuild();
Skill* PaperMeleeBuild();
Skill* PharaohRangedBuild();
Skill* PharaohSummonBuild();

void InitHeroSkills() {
    ugdk::base::ResourceContainer<Skill*>& container = RESOURCE_MANAGER()->get_container<skills::Skill*>();
    container.Insert("magic_missile", HeroMagicMissileBuild());
    container.Insert("fireball", HeroFireballBuild());
    container.Insert("lightning", HeroLightningBuild());
    container.Insert("light", HeroLightBuild());
}
void InitMummySkills() {
    ugdk::base::ResourceContainer<Skill*>& container = RESOURCE_MANAGER()->get_container<skills::Skill*>();
    container.Insert("mummy_melee", MummyMeleeBuild());
    container.Insert("mummy_ranged", MummyRangedBuild());
    container.Insert("paper_melee", PaperMeleeBuild());
    container.Insert("pharaoh_ranged", PharaohRangedBuild());
    container.Insert("pharaoh_summon", PharaohSummonBuild());
}

} // skills
