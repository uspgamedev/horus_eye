#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/base/engine.h>

#include "herometeorweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/explosion.h"
#include "game/sprites/timedworldobject.h"
#include "game/sprites/projectiles/carrier.h"
#include "game/sprites/scenery/wall.h"
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

void HeroMeteorWeapon::Attack(){
    World *world = WORLD();

    utils::ImageFactory *factory = world->image_factory();

    sprite::WorldObject *permanent_light = new sprite::WorldObject;
	permanent_light->Initialize(factory->LightImage());
	permanent_light->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y) );
	permanent_light->set_light_radius(4.0f);

    sprite::Explosion *explosion = new sprite::Explosion(factory->ExplosionImage(), 
        sprite::Explosion::HERO_FIREBALL_WEAPON, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::QUAKE_EXPLOSION_DAMAGE);

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    Vector2D nodir;
    sprite::Carrier *warning_effect = new sprite::Carrier(0.0f, 3000, nodir, list);
    warning_effect->Initialize(factory->LightImage());
    warning_effect->set_visible(false);

    world->AddWorldObject(warning_effect, cast_argument_.destination_);

    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    
    caster_mana_ -= mana_cost_;
}

HeroMeteorWeapon::HeroMeteorWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.LightningIconImage(); // TODO: change icon
}

bool HeroMeteorWeapon::Available() const {
    VisionStrategy vs;
    return CombatArt<castarguments::Aim>::Available() 
        && vs.IsVisible(cast_argument_.destination_, cast_argument_.origin_);
}

} // namespace skills
