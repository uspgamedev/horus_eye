
#include "game/map/room.h"
#include "game/components/caster.h"
#include "game/builders/projectilebuilder.h"
#include "game/skills/combatart.h"
#include "game/constants.h"

#include "communication/direct.h"

namespace skills {

using ugdk::math::Vector2D;

static void HeroLightningUse(component::Caster* caster) {
    //static Vector2D projectile_height = World::FromScreenCoordinates(Vector2D(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT));

    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
             pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::LightningBolt(versor), pos, map::POSITION_ABSOLUTE);

    communication::notify::PlaySound("samples/fire.wav");
}

Skill* HeroLightningBuild() {
    return new CombatArt(HeroLightningUse, SkillValidFunction(), "images/hud_magic_lightning.png", constants::GetInt("LIGHTNING_COST"));
}

} // namespace skills
