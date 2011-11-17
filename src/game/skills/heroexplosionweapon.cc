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

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

HeroExplosionWeapon::HeroExplosionWeapon(sprite::Hero* owner) 
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()) {

    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage();
}

void HeroExplosionWeapon::Attack(){
    InputManager *input_ = Engine::reference()->input_manager();
    World *world = WORLD();
    ImageFactory *imfac = world->image_factory();
    
    float distance = (cast_argument_.destination_ - cast_argument_.origin_).length();
    VisionStrategy vs;
    if (distance <= range() && vs.IsVisible(cast_argument_.destination_)) {
        sprite::Explosion* explosion = new sprite::Explosion(imfac->QuakeImage(),
                                              sprite::Explosion::HERO_EXPLOSION_WEAPON,
                                              Constants::QUAKE_EXPLOSION_RADIUS,
                                              Constants::QUAKE_EXPLOSION_DAMAGE);
        world->AddWorldObject(explosion, cast_argument_.destination_);
        utils::Settings settings;
        if(settings.sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

        caster_mana_ -= cost_;
    }

}

bool HeroExplosionWeapon::Available() const {
    float distance = (cast_argument_.destination_ - cast_argument_.origin_).length();
    return CombatArt<castarguments::Aim>::Available() && (distance <= range());
}

} // namespace skills
