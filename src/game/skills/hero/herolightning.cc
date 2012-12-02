#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "game/map/room.h"
#include "game/utils/hudimagefactory.h"
#include "game/components/caster.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/skills/combatart.h"
#include "game/constants.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;

static void HeroLightningUse(component::Caster* caster) {
    //static Vector2D projectile_height = World::FromScreenCoordinates(Vector2D(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT));

    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
             pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::LightningBolt(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroLightningBuild() {
    HudImageFactory factory;
    return new CombatArt(factory.LightningIconImage(), HeroLightningUse, constants::GetInt("LIGHTNING_COST"));
}

} // namespace skills
