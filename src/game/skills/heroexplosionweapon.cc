#include <ugdk/math/vector2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "heroexplosionweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/explosion.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

namespace skills {

using scene::World;
using utils::Constants;

const double HeroExplosionWeapon::range_ = utils::Constants::QUAKE_EXPLOSION_RANGE;

HeroExplosionWeapon::HeroExplosionWeapon(sprite::Hero* owner)
    : DivineGift<usearguments::Aim>(
        NULL, utils::Constants::QUAKE_COST, utils::Constants::QUAKE_BLOCK_COST, owner->mana(), owner->mana_blocks(), owner->aim()) {

    utils::HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage();
}

void HeroExplosionWeapon::Use() {
    super::Use();

    World *world = WORLD();
    sprite::Explosion* explosion = new sprite::Explosion(world->image_factory()->QuakeImage(),
                                            sprite::Explosion::HERO_EXPLOSION_WEAPON,
                                            Constants::QUAKE_EXPLOSION_RADIUS,
                                            Constants::QUAKE_EXPLOSION_DAMAGE);
    world->AddWorldObject(explosion, use_argument_.destination_);

    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

bool HeroExplosionWeapon::IsValidUse() const {
    utils::VisionStrategy vs;
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return super::IsValidUse()
        && (distance <= range_)
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
