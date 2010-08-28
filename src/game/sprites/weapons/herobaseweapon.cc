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

void HeroBaseWeapon::Attack(){

    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = ((World *)Engine::reference()->CurrentScene());
    // Ajuste da altura do projetil.
    //Vector2D mouseOffset = input_->GetMousePosition() - screen_center_ + Vector2D(0,Constants::PROJECTILE_SPRITE_CENTER_Y+Constants::PROJECTILE_HEIGHT);
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    Projectile * projectile = new Projectile(pos, versor);
    world_->AddWorldObject(projectile);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartAttack();
}

bool HeroBaseWeapon::Available() {

    return true;

}

}
