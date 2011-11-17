#include "herofireballweapon.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/base/engine.h>


#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/sprites/creatures/hero.h"

namespace sprite {
class Hero;
}

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroFireballWeapon::Attack() {
    InputManager *input_ = Engine::reference()->input_manager();

    Vector2D versor = (cast_argument_.destination_ - cast_argument_.origin_).Normalize(),
             pos = cast_argument_.origin_;

    World *world = WORLD();
    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.Fireball(versor), pos);


    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    caster_mana_ -= cost_;
}


HeroFireballWeapon::HeroFireballWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::FIREBALL_COST, owner->mana(), owner->aim()) {

    HudImageFactory factory;
    icon_ = factory.FireballIconImage();
}

}
