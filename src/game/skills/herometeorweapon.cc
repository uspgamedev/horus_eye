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
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D meteor_position = WORLD()->FromScreenCoordinates(input_->GetMousePosition());
    VisionStrategy vs;
    if (!vs.IsVisible(meteor_position))
        return;

    World *world = WORLD();

    utils::ImageFactory *factory = world->image_factory();

    sprite::WorldObject *permanent_light = new sprite::WorldObject;
	permanent_light->Initialize(factory->LightImage());
	permanent_light->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y) );
	permanent_light->set_light_radius(4.0f);

    sprite::Explosion *explosion = new sprite::Explosion(factory->ExplosionImage(), 
        sprite::Explosion::HERO_FIREBALL_WEAPON, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::FIREBALL_EXPLOSION_DAMAGE);

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    Vector2D nodir;
    sprite::Carrier *warning_effect = new sprite::Carrier(0.0f, 3000, nodir, list);
    warning_effect->Initialize(factory->LightImage());
    warning_effect->set_visible(false);

    world->AddWorldObject(warning_effect, meteor_position);

    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);
}

HeroMeteorWeapon::HeroMeteorWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::QUAKE_COST, owner->mana(), owner->aim()),
      hero_(owner)  { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.LightningIconImage(); // TODO: change icon
}

bool HeroMeteorWeapon::Available() const {
    return hero_->mana().Has(cost_);
}

}
