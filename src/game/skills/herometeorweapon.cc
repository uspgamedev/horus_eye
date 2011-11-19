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

void HeroMeteorWeapon::Use(){
    super::Use();
    //TODO: use meteor's constants.

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

    world->AddWorldObject(warning_effect, use_argument_.destination_);

    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

HeroMeteorWeapon::HeroMeteorWeapon(sprite::Hero* owner)
    : DivineGift<usearguments::Aim>(NULL, utils::Constants::METEOR_COST, utils::Constants::METEOR_BLOCK_COST, owner->mana(), owner->mana_blocks(), owner->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.LightningIconImage(); // TODO: change icon
}

bool HeroMeteorWeapon::IsValidUse() const {
    VisionStrategy vs;
    return CombatArt<usearguments::Aim>::IsValidUse() 
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
