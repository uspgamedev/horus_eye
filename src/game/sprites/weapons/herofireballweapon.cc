#include "herofireballweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/animation.h"
#include "../../../framework/engine.h"
#include "../../utils/imagefactory.h"
#include "../../utils/hudimagefactory.h"
#include "../../utils/constants.h"
#include "../../scenes/world.h"
#include "../explosion.h"
#include "../fireball.h"
#include "../../utils/settings.h"
#include "../hero.h"


namespace sprite {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroFireballWeapon::Attack() {
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    World *world_ = WORLD();
    ImageFactory *factory = world_->image_factory();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(WORLD()->FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    Explosion *explosion = new Explosion(factory->ExplosionImage(),
                                         Explosion::HERO_FIREBALL_WEAPON,
                                         Constants::FIREBALL_EXPLOSION_RADIUS,
                                         Constants::FIREBALL_EXPLOSION_DAMAGE);
    Fireball *fireball = new Fireball(versor, explosion);
    world_->AddWorldObject(fireball, pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);

}


HeroFireballWeapon::HeroFireballWeapon(Hero* owner) : Weapon(owner), hero_(owner), cost_(utils::Constants::FIREBALL_COST) {
    HudImageFactory factory;
    icon_ = factory.FireballIconImage();
}


bool HeroFireballWeapon::Available() {

    return hero_->mana() >= cost_;

}

}
