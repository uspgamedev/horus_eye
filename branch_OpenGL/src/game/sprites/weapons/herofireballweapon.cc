#include "herofireballweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/animation.h"
#include "../../../framework/engine.h"
#include "../../utils/imagefactory.h"
#include "../../utils/constants.h"
#include "../../scenes/world.h"
#include "../explosion.h"
#include "../fireball.h"

namespace sprite {

using namespace scene;
using namespace framework;
using namespace utils;
using utils::Constants;

void HeroFireballWeapon::Attack() {
    InputManager *input_ = Engine::reference()->input_manager();
    ImageFactory factory;
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();

    Animation *animation = new Animation(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1);
    Explosion *explosion = new Explosion(factory.ExplosionImage(), animation, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::FIREBALL_EXPLOSION_DAMAGE);
    Fireball *fireball = new Fireball(versor, explosion);
    world_->AddWorldObject(fireball, pos);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);
}

bool HeroFireballWeapon::Available() {

    return hero_->mana() >= cost_;

}

}