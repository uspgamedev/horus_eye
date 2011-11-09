#include "heroexplosionweapon.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/base/engine.h>
#include "game/scenes/world.h"
#include "game/sprites/explosion.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

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
