#include "game/skills/heroskills.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "game/map/room.h"
#include "game/sprites/worldobject.h"
#include "game/utils/constants.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

static bool VisibilityCheck(const component::Caster* caster) {
    VisionStrategy vs;
    return vs.IsVisible(caster->owner(), caster->aim().destination_);
}

static void HeroLightUse(component::Caster* caster){
    sprite::WorldObject *light = new sprite::WorldObject(5.0);
    light->node()->set_drawable(new ugdk::graphic::Sprite("light"));
    light->set_light_radius(4.0);

    caster->owner()->current_room()->AddObject(light, caster->aim().destination_, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroLight() {
    HudImageFactory imfac;
    return new CombatArt(imfac.LightIconImage(), HeroLightUse, VisibilityCheck,
        utils::Constants::QUAKE_COST, -1.0);
}

} // namespace skills
