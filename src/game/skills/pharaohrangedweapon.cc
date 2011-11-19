#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "pharaohrangedweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/mummy.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/skills/combatart.h"

namespace skills {

#define PI 3.1415926535897932384626433832795f

using ugdk::Vector2D;

const float PharaohRangedWeapon::range_ = utils::Constants::RANGED_MUMMY_RANGE;

void PharaohRangedWeapon::Use() {
    super::Use();

    scene::World *world = WORLD();
    sprite::Hero* hero = world->hero();

    //TODO:FIX 
    Vector2D pos = use_argument_.origin_;
    Vector2D distance = hero->world_position() - pos;
    
    float angle = atan2(1.5f, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    builder::ProjectileBuilder proj(world->image_factory());
    world->AddWorldObject(proj.MummyProjectile(versor, damage_), pos);
    world->AddWorldObject(proj.MummyProjectile(offsetleft, damage_), pos);
    world->AddWorldObject(proj.MummyProjectile(offsetright, damage_), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

bool PharaohRangedWeapon::IsValidUse() const {
    float distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance >= range_ / 2.0f) && (distance <= range_);
}

}
