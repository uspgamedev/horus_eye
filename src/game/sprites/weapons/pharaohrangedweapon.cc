#include <cmath>
#include "pharaohrangedweapon.h"
#include "../../scenes/world.h"
#include "../hero.h"
#include "../mummyprojectile.h"
#include "../../../framework/engine.h"
#include "../../../framework/audiomanager.h"

namespace sprite {

#define PI 3.1415926535897932384626433832795f

using framework::Vector2D;

void PharaohRangedWeapon::Attack(){
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D pos = owner_->world_position();
    Vector2D distance = hero->world_position() - pos;
    
    float angle = atan2(1.5f, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    world->AddWorldObject(new sprite::MummyProjectile(pos, versor, damage_));
    world->AddWorldObject(new sprite::MummyProjectile(pos, offsetleft, damage_));
    world->AddWorldObject(new sprite::MummyProjectile(pos, offsetright, damage_));
    framework::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    ((Mummy*)owner_)->StartAttack(hero);
}

}
