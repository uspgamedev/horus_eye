#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/base/engine.h>

#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/caster.h"
#include "game/utils/settings.h"
#include "game/skills/combatart.h"

namespace skills {

using ugdk::math::Vector2D;

static void HeroMagicMissileUse(component::Caster* caster) {
    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
             pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::MagicMissile(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        AUDIO_MANAGER()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroMagicMissileBuild() {
    return new CombatArt(NULL, HeroMagicMissileUse, 0);
}

}
