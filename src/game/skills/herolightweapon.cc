#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "herolightweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/components/logic/hero.h"
#include "game/utils/constants.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroLightWeapon::Use(){
    super::Use();

    World *world = WORLD();   

    sprite::WorldObject *light = new sprite::WorldObject(5.0);
    light->node()->set_drawable(new ugdk::graphic::Sprite("light"));
    light->set_light_radius(4.0);
    world->AddWorldObject(light, use_argument_.destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

HeroLightWeapon::HeroLightWeapon(component::Hero* owner)
    : CombatArt<usearguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->owner()->caster()->mana(), owner->owner()->caster()->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.LightIconImage(); // TODO: change icon
}

bool HeroLightWeapon::IsValidUse() const {
    VisionStrategy vs;
    return CombatArt<usearguments::Aim>::IsValidUse() 
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
