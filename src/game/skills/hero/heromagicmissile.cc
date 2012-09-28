#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>

#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/caster.h"
#include "game/utils/settings.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using utils::Constants;

static void HeroMagicMissileUse(component::Caster* caster) {
    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
             pos = caster->aim().origin_;

    builder::ProjectileBuilder proj;
    caster->owner()->current_room()->AddObject(proj.MagicMissile(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroMagicMissileBuild() {
    return new CombatArt(NULL, HeroMagicMissileUse, 0);
}

}
