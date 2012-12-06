#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/damageable.h"
#include "game/components/caster.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/skills/combatart.h"
#include "game/utils/settings.h"
#include "game/constants.h"

namespace skills {

using std::tr1::bind;
using namespace std::tr1::placeholders;

static void MummyMeleeUse(component::Caster* caster) {
    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(caster->power().Get() * 1.0);
}

static void MummyPaperUse(component::Caster* caster) {
    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(caster->power().Get() * 1.0);
    caster->owner()->damageable()->TakeDamage(caster->power().Get() * 1.0);
}

static void MummyRangedUse(component::Caster* caster) {
    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize();
    Vector2D pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::MummyProjectile(versor, caster->power().Get() * 1.0), pos, map::POSITION_ABSOLUTE);
    
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* MummyMeleeBuild() {
    return new CombatArt(NULL, MummyMeleeUse, SkillValidFunction(), 0.0, 1.0);
}

Skill* MummyRangedBuild() {
    return new CombatArt(NULL, MummyRangedUse, SkillValidFunction(), 0.0, 
        constants::GetDouble("RANGED_MUMMY_RANGE"));
}

Skill* PaperMeleeBuild() {
    return new CombatArt(NULL, MummyPaperUse, SkillValidFunction(), 0.0, 1.0);
}

}
