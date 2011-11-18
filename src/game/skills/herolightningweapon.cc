#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/base/engine.h>

#include "herolightningweapon.h"

#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/sprites/creatures/hero.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroLightningWeapon::Attack() {
    //static Vector2D projectile_height = World::FromScreenCoordinates(Vector2D(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT));
    
    Vector2D versor = (cast_argument_.destination_ /*+ projectile_height*/ - cast_argument_.origin_).Normalize(),
             pos = cast_argument_.origin_;

    World *world = WORLD();

    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.LightningBolt(versor), pos);


    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    
    caster_mana_ -= mana_cost_;
}


HeroLightningWeapon::HeroLightningWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::LIGHTNING_COST, owner->mana(), owner->aim()) {

    HudImageFactory factory;
    icon_ = factory.LightningIconImage();
}

} // namespace skills
