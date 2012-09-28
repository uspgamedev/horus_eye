#include "game/skills/heroskills.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>

namespace skills {

Skill* HeroMagicMissileBuild();
Skill* HeroFireballBuild();
Skill* HeroLightningBuild();
Skill* HeroLightBuild();
Skill* HeroQuakeBuild();
Skill* HeroMeteorBuild();
Skill* HeroSandstormBuild();

void InitHeroSkills() {
    ugdk::base::ResourceContainer<Skill*>& container = RESOURCE_MANAGER()->get_container<skills::Skill*>();
    container.Insert("magic_missile", HeroMagicMissileBuild());
    container.Insert("fireball", HeroFireballBuild());
    container.Insert("lightning", HeroLightningBuild());
    container.Insert("light", HeroLightBuild());
    container.Insert("quake", HeroQuakeBuild());
    container.Insert("meteor", HeroMeteorBuild());
    container.Insert("sandstorm", HeroSandstormBuild());
}

Skill* HeroMagicMissile() { 
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("magic_missile");
}
Skill* HeroFireball() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("fireball");
}
Skill* HeroLightning() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("lightning");
}
Skill* HeroLight() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("light");
}
Skill* HeroQuake() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("quake");
}
Skill* HeroMeteor() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("meteor");
}
Skill* HeroSandstorm() {
    return RESOURCE_MANAGER()->get_container<skills::Skill*>().Find("sandstorm");
}


} // skills
