#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>

#include "game/builders/explosionbuilder.h"
#include "game/map/room.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"
#include "game/components/graphic.h"
#include "game/builders/functions/carrier.h"
#include "game/skills/divinegift.h"
#include "game/constants.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using sprite::WorldObject;

static bool VisibilityCheck(const component::Caster* caster) {
    utils::VisionStrategy vs;
    return vs.IsVisible(caster->owner(), caster->aim().destination_);
}

static void HeroMeteorUse(component::Caster* caster) {
    sprite::WorldObject *permanent_light = new sprite::WorldObject;
    permanent_light->graphic()->ChangeLightRadius(4.0);

    sprite::WorldObject *explosion = builder::ExplosionBuilder::MeteorExplosion();

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    WorldObject* warning_effect = new WorldObject(3.0);
    warning_effect->set_start_to_die_callback(builder::function::Carrier(list));

    caster->owner()->current_room()->AddObject(warning_effect, caster->aim().destination_, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroMeteorBuild() {
    utils::HudImageFactory imfac;
    return new DivineGift(imfac.MeteorIconImage(), HeroMeteorUse, VisibilityCheck,
        constants::GetInt("METEOR_COST"), constants::GetInt("METEOR_BLOCK_COST"), -1);
}

} // namespace skills
