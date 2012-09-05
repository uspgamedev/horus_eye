#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>

#include "game/skills/herobaseweapon.h"

#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/logic/creature.h"
#include "game/utils/settings.h"
#include "game/utils/constants.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using utils::Constants;

HeroBaseWeapon::HeroBaseWeapon(component::Creature* owner) 
    : CombatArt<usearguments::Aim>(NULL, 0.0, owner->owner()->caster()->mana(), owner->owner()->caster()->aim()) {}

void HeroBaseWeapon::Use(){
    super::Use();

    World *world = WORLD();

    Vector2D versor = (use_argument_.destination_ - use_argument_.origin_).Normalize(),
             pos = use_argument_.origin_;
    builder::ProjectileBuilder proj;
    world->AddWorldObject(proj.MagicMissile(versor), pos);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

}
