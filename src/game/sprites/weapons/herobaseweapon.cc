#include "herobaseweapon.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>
#include "../../scenes/world.h"
#include "../magicmissile.h"
#include "../../utils/settings.h"

namespace sprite {

using namespace scene;
using namespace ugdk;
using utils::Constants;

void HeroBaseWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    MagicMissile * projectile = new MagicMissile(versor);
    world_->AddWorldObject(projectile, pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartAttack();
}

bool HeroBaseWeapon::Available() { return true; }

}
