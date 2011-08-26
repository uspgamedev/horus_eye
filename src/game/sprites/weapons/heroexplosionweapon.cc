#include "heroexplosionweapon.h"
#include "ugdk/vector2D.h"
#include "ugdk/inputmanager.h"
#include "ugdk/audiomanager.h"
#include "ugdk/animation.h"
#include "ugdk/engine.h"
#include "../../scenes/world.h"
#include "../explosion.h"
#include "../../utils/visionstrategy.h"
#include "../../utils/imagefactory.h"
#include "../../utils/hudimagefactory.h"
#include "../../utils/constants.h"
#include "../../utils/settings.h"

namespace sprite {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroExplosionWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    World *world = WORLD();
    ImageFactory *imfac = world->image_factory();
    
    Vector2D explosionPosition = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    float distance = (hero_->world_position() - explosionPosition).length();
    VisionStrategy vs;
    if (distance <= range() && vs.IsVisible(explosionPosition)) {
        Explosion * explosion = new Explosion(imfac->QuakeImage(),
                                              Explosion::HERO_EXPLOSION_WEAPON,
                                              Constants::QUAKE_EXPLOSION_RADIUS,
                                              Constants::QUAKE_EXPLOSION_DAMAGE);
        world->AddWorldObject(explosion, explosionPosition);
        utils::Settings settings;
        if(settings.sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
        hero_->StartExplosion();
        hero_->set_mana(hero_->mana() - cost_);
    }

}

HeroExplosionWeapon::HeroExplosionWeapon(Hero* owner) : Weapon(owner), hero_(owner), cost_(utils::Constants::QUAKE_COST) {
    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage();
}

bool HeroExplosionWeapon::Available() {
    return hero_->mana() >= cost_;
}

}
