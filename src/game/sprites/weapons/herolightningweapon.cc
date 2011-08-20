#include "herolightningweapon.h"
#include "../../../framework/vector2D.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/audiomanager.h"
#include "../../../framework/animation.h"
#include "../../../framework/engine.h"
#include "../../utils/imagefactory.h"
#include "../../utils/hudimagefactory.h"
#include "../../utils/constants.h"
#include "../../scenes/world.h"
#include "../lightningbolt.h"
#include "../../utils/settings.h"
#include "../hero.h"


namespace sprite {

using namespace scene;
using namespace framework;
using namespace utils;
using utils::Constants;

void HeroLightningWeapon::Attack() {
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = Vector2D::Normalized(World::FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()),
             pos = hero_->world_position();
    LightningBolt *lightning = new LightningBolt(versor);
    world_->AddWorldObject(lightning, pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);

}


HeroLightningWeapon::HeroLightningWeapon(Hero* owner) : Weapon(owner), hero_(owner), 
        cost_(utils::Constants::LIGHTNING_COST) {
    HudImageFactory factory;
    icon_ = factory.LightningIconImage();
}


bool HeroLightningWeapon::Available() {
    return hero_->mana() >= cost_;
}

}
