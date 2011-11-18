#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>

#include "game/skills/herobaseweapon.h"

#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/settings.h"
#include "game/utils/constants.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using utils::Constants;

HeroBaseWeapon::HeroBaseWeapon(sprite::Hero* owner) 
    : CombatArt<castarguments::Aim>(NULL, 0.0f, owner->mana(), owner->aim()) {}

void HeroBaseWeapon::Attack(){
    World *world = WORLD();

    Vector2D versor = (cast_argument_.destination_ - cast_argument_.origin_).Normalize(),
             pos = cast_argument_.origin_;
    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.MagicMissile(versor), pos);

    caster_mana_ -= mana_cost_;

    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

}
