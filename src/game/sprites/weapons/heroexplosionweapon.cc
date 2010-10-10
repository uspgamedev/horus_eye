#include "heroexplosionweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/animation.h"
#include "../../../framework/engine.h"
#include "../../scenes/world.h"
#include "../explosion.h"
#include "../../utils/visionstrategy.h"
#include "../../utils/imagefactory.h"
#include "../../utils/constants.h"
#include "../../utils/settings.h"

namespace sprite {

using namespace scene;
using namespace framework;
using namespace utils;
using utils::Constants;

void HeroExplosionWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    World *world_ = WORLD();
    ImageFactory *imfac = world_->image_factory();
    
    Vector2D explosionPosition = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    float distance = (hero_->world_position() - explosionPosition).length();
    VisionStrategy vs;
    if (distance <= 5 && vs.IsVisible(explosionPosition)) {
        Animation *animation = new Animation(8, 0, 1, 2, 3, 4, 5, -1);
        Explosion * explosion = new Explosion(imfac->QuakeImage(), animation, Constants::QUAKE_EXPLOSION_RADIUS, Constants::QUAKE_EXPLOSION_DAMAGE);
        world_->AddWorldObject(explosion, explosionPosition);
        utils::Settings settings;
        if(settings.sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
        hero_->StartExplosion();
        hero_->set_mana(hero_->mana() - cost_);
    }
}

bool HeroExplosionWeapon::Available() {
    return hero_->mana() >= cost_;
}

}
