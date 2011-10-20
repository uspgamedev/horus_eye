#include "mummyrangedweapon.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/mummy.h"
#include "../mummyprojectile.h"
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include "game/utils/settings.h"

namespace sprite {

using ugdk::Vector2D;

void MummyRangedWeapon::Attack(){
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D versor = Vector2D::Normalized(hero->world_position() - owner_->world_position());
    Vector2D pos = owner_->world_position();

    world->AddWorldObject(new sprite::MummyProjectile(versor, damage_), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    ((Mummy*)owner_)->StartAttack(hero);
}

}
