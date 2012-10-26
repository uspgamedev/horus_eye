#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "game/builders/explosionbuilder.h"
#include "game/map/room.h"
#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/settings.h"
#include "game/skills/divinegift.h"
#include "game/constants.h"

namespace skills {

using scene::World;

static bool VisibilityCheck(const component::Caster* caster) {
    utils::VisionStrategy vs;
    return vs.IsVisible(caster->owner(), caster->aim().destination_);
}

static void HeroQuakeUse(component::Caster* caster) {
    builder::ExplosionBuilder builder;
    caster->owner()->current_room()->AddObject(builder.EarthquakeExplosion(), caster->aim().destination_, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroQuakeBuild() {
    utils::HudImageFactory imfac;
    return new DivineGift(imfac.EarthquakeIconImage(), HeroQuakeUse, VisibilityCheck,
        constants::GetInt("QUAKE_COST"), constants::GetInt("QUAKE_BLOCK_COST"), constants::GetDouble("QUAKE_EXPLOSION_RANGE"));
}

} // namespace skills
