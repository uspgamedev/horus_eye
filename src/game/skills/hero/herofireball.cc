#include "game/skills/combatart.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/audio/module.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/system/engine.h>

#include "game/constants.h"
#include "game/utils/hudimagefactory.h"
#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"

namespace skills {

using ugdk::math::Vector2D;

static void HeroFireballUse(component::Caster* caster) {
    //static Vector2D projectile_height = World::FromScreenLinearCoordinates(Vector2D(0,constants::GetDouble("FIREBALL_SPRITE_CENTER_Y")+constants::GetDouble("FIREBALL_HEIGHT")));

    Vector2D versor = (caster->aim().destination_ - caster->aim().origin_).Normalize(),
        pos = caster->aim().origin_;

    caster->owner()->current_room()->AddObject(builder::ProjectileBuilder::Fireball(versor), pos, map::POSITION_ABSOLUTE);

    if(utils::Settings::reference()->sound_effects())
        ugdk::audio::manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroFireballBuild() {
    return new CombatArt(HeroFireballUse, SkillValidFunction(), [] {
        return std::unique_ptr<ugdk::ui::Drawable>(utils::HudImageFactory::FireballIconImage());
    }, constants::GetInt("FIREBALL_COST"));
}

} // namespace skills
