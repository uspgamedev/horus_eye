#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>

#include "herobaseweapon.h"

#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"

namespace sprite {

using namespace scene;
using namespace ugdk;
using utils::Constants;

void HeroBaseWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = (WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()).Normalize(),
             pos = hero_->world_position();

    builder::ProjectileBuilder proj(world_->image_factory());
    world_->AddWorldObject(proj.MagicMissile(versor), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartAttack();
}

bool HeroBaseWeapon::Available() { return true; }

}
