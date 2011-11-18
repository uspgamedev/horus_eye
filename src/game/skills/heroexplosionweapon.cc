#include <ugdk/math/vector2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "heroexplosionweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/explosion.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

namespace skills {

using scene::World;
using namespace utils;
using utils::Constants;


HeroExplosionWeapon::HeroExplosionWeapon(sprite::Hero* owner)
    : DivineGift<castarguments::Aim>(
        NULL, utils::Constants::QUAKE_COST, utils::Constants::QUAKE_BLOCK_COST, owner->mana(), owner->mana_blocks(), owner->aim()) {

    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage();
}

void HeroExplosionWeapon::Attack() {

    super::Attack();

    World *world = WORLD();
    sprite::Explosion* explosion = new sprite::Explosion(world->image_factory()->QuakeImage(),
                                            sprite::Explosion::HERO_EXPLOSION_WEAPON,
                                            Constants::QUAKE_EXPLOSION_RADIUS,
                                            Constants::QUAKE_EXPLOSION_DAMAGE);
    world->AddWorldObject(explosion, cast_argument_.destination_);

    utils::Settings settings;
    if(settings.sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

bool HeroExplosionWeapon::Available() const {
    VisionStrategy vs;
    float distance = (cast_argument_.destination_ - cast_argument_.origin_).length();
    return DivineGift<castarguments::Aim>::Available() 
        && (distance <= range()) 
        && vs.IsVisible(cast_argument_.destination_, cast_argument_.origin_);
}

} // namespace skills
