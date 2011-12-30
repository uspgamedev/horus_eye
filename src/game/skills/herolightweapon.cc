#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/base/engine.h>

#include "herolightweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/explosion.h"
#include "game/sprites/timedworldobject.h"
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

    sprite::WorldObject *light = new sprite::TimedWorldObject(5.0f);
    light->node()->set_drawable(new Sprite(world->image_factory()->LightImage()));
    world->image_factory()->LightImage()->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y) );
    light->set_light_radius(4.0f);
    world->AddWorldObject(light, use_argument_.destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

HeroLightWeapon::HeroLightWeapon(sprite::Hero* owner)
    : CombatArt<usearguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage(); // TODO: change icon
}

bool HeroLightWeapon::IsValidUse() const {
    VisionStrategy vs;
    return CombatArt<usearguments::Aim>::IsValidUse() 
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
