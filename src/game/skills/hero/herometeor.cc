#include "game/skills/heroskills.h"

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>

#include "game/builders/explosionbuilder.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"
#include "game/builders/functions/carrier.h"
#include "game/skills/divinegift.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;
using sprite::WorldObject;

static bool VisibilityCheck(const component::Caster* caster) {
    utils::VisionStrategy vs;
    return vs.IsVisible(caster->aim().destination_, caster->aim().origin_);
}

static void HeroMeteorUse(component::Caster* caster) {
    World *world = WORLD();

    sprite::WorldObject *permanent_light = new sprite::WorldObject;
    permanent_light->set_light_radius(4.0);

    builder::ExplosionBuilder builder;
    sprite::WorldObject *explosion = builder.MeteorExplosion();

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    WorldObject* warning_effect = new WorldObject(3.0);
    warning_effect->set_start_to_die_callback(builder::function::Carrier(list));

    world->AddWorldObject(warning_effect, caster->aim().destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroMeteor() {
    utils::HudImageFactory imfac;
    return new DivineGift(imfac.MeteorIconImage(), HeroMeteorUse, VisibilityCheck,
        utils::Constants::METEOR_COST, utils::Constants::METEOR_BLOCK_COST, -1);
}

} // namespace skills
