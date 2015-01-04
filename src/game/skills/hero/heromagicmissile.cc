
#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/caster.h"
#include "game/skills/combatart.h"

#include "communication/direct.h"

namespace skills {

using ugdk::math::Vector2D;

static void HeroMagicMissileUse(component::Caster* caster) {
    Vector2D versor = caster->aim().direction(),
             pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::MagicMissile(versor), pos, map::POSITION_ABSOLUTE);

    communication::notify::PlaySound("samples/fire.wav");
}

Skill* HeroMagicMissileBuild() {
    return new CombatArt(HeroMagicMissileUse, SkillValidFunction(), "", 0.0);
}

}
