#include "pharaohrangedweapon.h"
#include "../../scenes/world.h"
#include "../hero.h"
#include "../mummyprojectile.h"
#include "../../../framework/engine.h"
#include "../../../framework/audiomanager.h"

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059f

using framework::Vector2D;

void PharaohRangedWeapon::Attack(){
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D pos = owner_->world_position();
    Vector2D versor = Vector2D::Normalized(hero->world_position() - pos);
    Vector2D offsetleft = Vector2D(versor.x * SQRT_3/2.0f - versor.y * 1.0f/2.0f,
                                   versor.x * 1.0f/2.0f   + versor.y * SQRT_3/2.0f);
    Vector2D offsetright = Vector2D(versor.x * SQRT_3/2.0f + versor.y * 1.0f/2.0f,
                                    -versor.x * 1.0f/2.0f  + versor.y * SQRT_3/2.0f);

    world->AddWorldObject(new sprite::MummyProjectile(pos, versor, damage_));
    world->AddWorldObject(new sprite::MummyProjectile(pos, offsetleft, damage_));
    world->AddWorldObject(new sprite::MummyProjectile(pos, offsetright, damage_));
    framework::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    ((Mummy*)owner_)->StartAttack(hero);
}

}
