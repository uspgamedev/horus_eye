#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>

#include "pharaohrangedweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/mummy.h"
#include "game/sprites/projectiles/mummyprojectile.h"
#include "game/utils/settings.h"

namespace sprite {

#define PI 3.1415926535897932384626433832795f

using ugdk::Vector2D;

void PharaohRangedWeapon::Attack(){
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D pos = owner_->world_position();
    Vector2D distance = hero->world_position() - pos;
    
    float angle = atan2(1.5f, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    world->AddWorldObject(new sprite::MummyProjectile(versor, damage_), pos);
    world->AddWorldObject(new sprite::MummyProjectile(offsetleft, damage_), pos);
    world->AddWorldObject(new sprite::MummyProjectile(offsetright, damage_), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    ((Mummy*)owner_)->StartAttack(hero);
}

}
