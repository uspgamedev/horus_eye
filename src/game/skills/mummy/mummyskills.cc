#include "game/skills/mummyskills.h"

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
#include "game/utils/constants.h"

namespace skills {

using std::tr1::bind;
using namespace std::tr1::placeholders;
using utils::Constants;

static void MummyMeleeUse(component::Caster* caster, double damage) {
    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(damage);
}

static void MummyPaperUse(component::Caster* caster, double damage) {
    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(damage);
    caster->owner()->damageable()->TakeDamage(damage);
}

static void MummyRangedUse(component::Caster* caster, double damage) {
    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize();
    Vector2D pos = caster->aim().origin_;

    builder::ProjectileBuilder proj;
    caster->owner()->current_room()->AddObject(proj.MummyProjectile(versor, damage), pos, map::POSITION_ABSOLUTE);
    
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* MummyMelee(double damage) {
    return new CombatArt(NULL, bind(MummyMeleeUse, _1, damage), SkillValidFunction(), 0.0, 1.0);
}

Skill* MummyRanged(double damage) {
    return new CombatArt(NULL, bind(MummyRangedUse, _1, damage), SkillValidFunction(), 0.0, 
        Constants::RANGED_MUMMY_RANGE);
}

Skill* PaperMelee(double damage) {
    return new CombatArt(NULL, bind(MummyPaperUse, _1, damage), SkillValidFunction(), 0.0, 1.0);
}

}
