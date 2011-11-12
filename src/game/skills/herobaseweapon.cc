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
    : CombatArt(NULL, owner->aim()) {}

void HeroBaseWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    // TODO: ask the mouse postition from hero's Aim.
    Vector2D versor = (WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-cast_argument_->origin).Normalize(),
             pos = cast_argument_->origin;

    builder::ProjectileBuilder proj(world_->image_factory());
    world_->AddWorldObject(proj.MagicMissile(versor), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

bool HeroBaseWeapon::Available() const { return true; }

}
