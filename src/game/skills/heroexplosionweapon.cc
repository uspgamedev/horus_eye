#include <ugdk/math/vector2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "heroexplosionweapon.h"

#include "game/builders/explosionbuilder.h"
#include "game/scenes/world.h"
#include "game/components/logic/hero.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

namespace skills {

using scene::World;
using utils::Constants;

const double HeroExplosionWeapon::range_ = utils::Constants::QUAKE_EXPLOSION_RANGE;

HeroExplosionWeapon::HeroExplosionWeapon(component::Hero* owner)
    : DivineGift<usearguments::Aim>(
        NULL, utils::Constants::QUAKE_COST, utils::Constants::QUAKE_BLOCK_COST,
				owner->owner()->caster()->mana(), owner->owner()->caster()->mana_blocks(), owner->owner()->caster()->aim()) {

    utils::HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage();
}

void HeroExplosionWeapon::Use() {
    super::Use();

    World *world = WORLD();
    builder::ExplosionBuilder builder;
    world->AddWorldObject(builder.EarthquakeExplosion(), use_argument_.destination_);

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
