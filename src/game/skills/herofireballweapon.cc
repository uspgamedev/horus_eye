#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "herofireballweapon.h"

#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/components/logic/hero.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroFireballWeapon::Use() {
    super::Use();
    //static Vector2D projectile_height = World::FromScreenLinearCoordinates(Vector2D(0,Constants::FIREBALL_SPRITE_CENTER_Y+Constants::FIREBALL_HEIGHT));

    Vector2D versor = (use_argument_.destination_ /*+ projectile_height*/ - use_argument_.origin_).Normalize(),
             pos = use_argument_.origin_;

    World *world = WORLD();

    builder::ProjectileBuilder proj;
    world->AddWorldObject(proj.Fireball(versor), pos);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}


HeroFireballWeapon::HeroFireballWeapon(component::Caster* caster)
    : CombatArt<usearguments::Aim>(NULL, utils::Constants::FIREBALL_COST, caster->mana(), caster->aim()) {

    HudImageFactory factory;
    icon_ = factory.FireballIconImage();
}

} // namespace skills
