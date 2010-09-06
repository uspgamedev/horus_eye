#include "mummyrangedweapon.h"
#include "../../scenes/world.h"
#include "../hero.h"
#include "../mummyprojectile.h"
#include "../../../framework/engine.h"
#include "../../../framework/audiomanager.h"

namespace sprite {

using framework::Vector2D;

void MummyRangedWeapon::Attack(){
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D versor = Vector2D::Normalized(hero->world_position() - owner_->world_position());
    Vector2D pos = owner_->world_position();

    world->AddWorldObject(new sprite::MummyProjectile(versor, damage_), pos);
    framework::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    ((Mummy*)owner_)->StartAttack(hero);
}

}
