#include "herofireballweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/engine.h"
#include "../../scenes/world.h"
#include "../explosion.h"

namespace sprite {

using namespace scene;
using namespace framework;
using utils::Constants;

void HeroFireballWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    Explosion * explosion = new Explosion(pos, versor);
    world_->AddWorldObject(explosion);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);
}

bool HeroFireballWeapon::Available() {

    return hero_->mana() >= cost_;

}

}
