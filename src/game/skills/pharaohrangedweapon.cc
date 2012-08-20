#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "pharaohrangedweapon.h"

#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/skills/combatart.h"

namespace skills {

#define PI 3.1415926535897932384626433832795

using ugdk::Vector2D;

const double PharaohRangedWeapon::range_ = utils::Constants::RANGED_MUMMY_RANGE;

void PharaohRangedWeapon::Use() {
    super::Use();

    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero_world_object();

    //TODO:FIX 
    Vector2D pos = use_argument_.origin_;
    Vector2D distance = hero->world_position() - pos;
    
    double angle = atan2(1.5, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    builder::ProjectileBuilder proj;
    world->AddWorldObject(proj.MummyProjectile(versor, damage_), pos);
    world->AddWorldObject(proj.MummyProjectile(offsetleft, damage_), pos);
    world->AddWorldObject(proj.MummyProjectile(offsetright, damage_), pos);
    
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

bool PharaohRangedWeapon::IsValidUse() const {
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance >= range_ / 2.0) && (distance <= range_);
}

}
