#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "mummyrangedweapon.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/mummy.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"

namespace sprite {
class Mummy;
class Hero;
}

namespace skills {

using ugdk::Vector2D;

void MummyRangedWeapon::Use(){
    scene::World* world = WORLD();
    sprite::Hero* hero = world->hero();

    //TODO:FIX
    Vector2D versor = (hero->world_position() - use_argument_.origin_).Normalize();
    Vector2D pos = use_argument_.origin_;

    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.MummyProjectile(versor, damage_), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

}
