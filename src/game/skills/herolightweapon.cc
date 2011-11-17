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
    InputManager *input_ = Engine::reference()->input_manager();
    World *world = WORLD();
    ImageFactory *imfac = world->image_factory();
    
    Vector2D light_position = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    VisionStrategy vs;
    if (vs.IsVisible(light_position)) {
		sprite::WorldObject *light = new sprite::TimedWorldObject(5.0f);
		light->Initialize(world->image_factory()->LightImage());
		light->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y) );
		light->set_light_radius(4.0f);
		world->AddWorldObject(light, light_position);

        utils::Settings settings;
        if(settings.sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
        hero_->StartExplosion();
        hero_->set_mana(hero_->mana() - cost_);
    }

}

HeroLightWeapon::HeroLightWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()),
      hero_(owner)  { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.EarthquakeIconImage(); // TODO: change icon
}

bool HeroLightWeapon::Available() const {
    return hero_->mana().Has(cost_);
}

}
