#include "game/skills/heroskills.h"

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "game/builders/explosionbuilder.h"
#include "game/map/room.h"
#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/skills/divinegift.h"

namespace skills {

using scene::World;
using utils::Constants;

static bool VisibilityCheck(const component::Caster* caster) {
    utils::VisionStrategy vs;
    return vs.IsVisible(caster->aim().destination_, caster->aim().origin_);
}

static void HeroQuakeUse(component::Caster* caster) {
    builder::ExplosionBuilder builder;
    caster->owner()->current_room()->AddObject(builder.EarthquakeExplosion(), caster->aim().destination_);

    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroQuake() {
    utils::HudImageFactory imfac;
    return new DivineGift(imfac.EarthquakeIconImage(), HeroQuakeUse, VisibilityCheck,
        utils::Constants::QUAKE_COST, utils::Constants::QUAKE_BLOCK_COST, utils::Constants::QUAKE_EXPLOSION_RANGE);
}

} // namespace skills
