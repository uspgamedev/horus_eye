#include "heroexplosionweapon.h"
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
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D explosionPosition = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    Explosion * explosion = new Explosion();
    world_->AddWorldObject(explosion, explosionPosition);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);
}

bool HeroFireballWeapon::Available() {

    return hero_->mana() >= cost_;

}

}
