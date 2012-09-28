#include "game/skills/heroskills.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

static void HeroFireballUse(component::Caster* caster) {
    //static Vector2D projectile_height = World::FromScreenLinearCoordinates(Vector2D(0,Constants::FIREBALL_SPRITE_CENTER_Y+Constants::FIREBALL_HEIGHT));

    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
        pos = caster->aim().origin_;

    builder::ProjectileBuilder proj;
    caster->owner()->current_room()->AddObject(proj.Fireball(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroFireballBuild() {
    HudImageFactory factory;
    return new CombatArt(factory.FireballIconImage(), HeroFireballUse, utils::Constants::FIREBALL_COST);
}

} // namespace skills
