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

void HeroLightningWeapon::Use() {
    super::Use();
    //static Vector2D projectile_height = World::FromScreenCoordinates(Vector2D(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT));

    Vector2D versor = (use_argument_.destination_ /*+ projectile_height*/ - use_argument_.origin_).Normalize(),
             pos = use_argument_.origin_;

    World *world = WORLD();

    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.LightningBolt(versor), pos);


    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}


HeroLightningWeapon::HeroLightningWeapon(sprite::Hero* owner)
    : CombatArt<usearguments::Aim>(NULL, utils::Constants::LIGHTNING_COST, owner->mana(), owner->aim()) {

    HudImageFactory factory;
    icon_ = factory.LightningIconImage();
}

} // namespace skills
