
#include "game/map/room.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/components/caster.h"
#include "game/components/graphic.h"
#include "game/components/timedlife.h"
#include "game/components/lightemitter.h"
#include "game/skills/combatart.h"
#include "game/constants.h"

#include "communication/direct.h"

namespace skills {

using namespace ugdk;
using namespace utils;

static bool VisibilityCheck(const component::Caster* caster) {
    VisionStrategy vs;
    return vs.IsVisible(caster->owner(), caster->aim().destination_);
}

static void HeroLightUse(component::Caster* caster) {
    sprite::WObjPtr light = sprite::WorldObject::Create();

    light->AddComponent(component::Graphic::CreateWithSingleFrame("projectile", "yellow-ball"));
    light->AddComponent(new component::LightEmitter(4.0));
    light->AddComponent(new component::TimedLife(15.0));

    caster->owner()->current_room()->AddObject(light, caster->aim().destination_, map::POSITION_ABSOLUTE);

    communication::notify::PlaySound("samples/fire.wav");
}

Skill* HeroLightBuild() {
    return new CombatArt(HeroLightUse, VisibilityCheck, "images/hud_magic_light.png", constants::GetInt("QUAKE_COST"), -1.0);
}

} // namespace skills
