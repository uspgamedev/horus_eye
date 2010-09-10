#include "heroexplosionweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/engine.h"
#include "../../scenes/world.h"
#include "../explosion.h"
#include "../../utils/visionstrategy.h"

namespace sprite {

using namespace scene;
using namespace framework;
using namespace utils;
using utils::Constants;

void HeroExplosionWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    World *world_ = WORLD();
    Vector2D explosionPosition = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    float distance = (hero_->world_position() - explosionPosition).length();
    VisionStrategy vs;
    if (distance <= 5 && vs.IsVisible(explosionPosition)) {
        Explosion * explosion = new Explosion();
        world_->AddWorldObject(explosion, explosionPosition);
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
        hero_->StartExplosion();
        hero_->set_mana(hero_->mana() - cost_);
    }
}

bool HeroExplosionWeapon::Available() {
    return hero_->mana() >= cost_;
}

}
