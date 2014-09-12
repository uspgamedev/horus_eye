#include <ugdk/math/vector2D.h>
#include <ugdk/audio/module.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/system/engine.h>

#include "game/map/room.h"
#include "game/utils/hudimagefactory.h"
#include "game/components/caster.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/skills/combatart.h"
#include "game/constants.h"

namespace skills {

using ugdk::math::Vector2D;

static void HeroLightningUse(component::Caster* caster) {
    //static Vector2D projectile_height = World::FromScreenCoordinates(Vector2D(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT));

    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
             pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::LightningBolt(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        ugdk::audio::manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroLightningBuild() {
    return new CombatArt(HeroLightningUse, SkillValidFunction(), [] {
        return std::unique_ptr<ugdk::ui::Drawable>(utils::HudImageFactory::LightningIconImage());
    }, constants::GetInt("LIGHTNING_COST"));
}

} // namespace skills
