#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "mummyrangedweapon.h"
#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"

namespace sprite {
class Mummy;
class Hero;
}

namespace skills {

using ugdk::Vector2D;

const double MummyRangedWeapon::range_ = utils::Constants::RANGED_MUMMY_RANGE;

void MummyRangedWeapon::Use(){
    super::Use();

    scene::World* world = WORLD();
    sprite::WorldObject* hero = world->hero_world_object();

    //TODO:FIX
    Vector2D versor = (hero->world_position() - use_argument_.origin_).Normalize();
    Vector2D pos = use_argument_.origin_;

    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.MummyProjectile(versor, damage_), pos);
    
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

bool MummyRangedWeapon::IsValidUse() const {
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance <= range_);
}

}
