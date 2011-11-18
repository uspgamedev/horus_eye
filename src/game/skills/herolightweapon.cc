#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
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

void HeroLightWeapon::Attack(){
    World *world = WORLD();   

	sprite::WorldObject *light = new sprite::TimedWorldObject(5.0f);
	light->Initialize(world->image_factory()->LightImage());
	light->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y) );
	light->set_light_radius(4.0f);
	world->AddWorldObject(light, cast_argument_.destination_);

    caster_mana_ -= mana_cost_;

    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

HeroLightWeapon::HeroLightWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage(); // TODO: change icon
}

bool HeroLightWeapon::Available() const {
    VisionStrategy vs;
    return CombatArt<castarguments::Aim>::Available() 
        && vs.IsVisible(cast_argument_.destination_, cast_argument_.origin_);
}

} // namespace skills