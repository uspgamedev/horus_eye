#include "herobaseweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/engine.h"
#include "../../scenes/world.h"
#include "../projectile.h"

namespace sprite {

using namespace scene;
using namespace framework;
using utils::Constants;

void HeroBaseWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    Projectile * projectile = new Projectile(versor);
    world_->AddWorldObject(projectile, pos);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartAttack();
}

bool HeroBaseWeapon::Available() {

    return true;

}

}
