#include "herolightningweapon.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/base/engine.h>
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/scenes/world.h"
#include "game/builders/projectilebuilder.h"
#include "game/utils/settings.h"
#include "game/sprites/creatures/hero.h"


namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

void HeroLightningWeapon::Attack() {
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::LIGHTNING_SPRITE_CENTER_Y+Constants::LIGHTNING_HEIGHT);
    World *world_ = WORLD();
    // Ajuste da altura do projetil.
    Vector2D versor = (World::FromScreenCoordinates(input_->GetMousePosition() + projectile_height)-hero_->world_position()).Normalize(),
             pos = hero_->world_position();
    builder::ProjectileBuilder proj(world_->image_factory());
    world_->AddWorldObject(proj.LightningBolt(versor), pos);
    utils::Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    hero_->StartExplosion();
    hero_->set_mana(hero_->mana() - cost_);

}


HeroLightningWeapon::HeroLightningWeapon(sprite::Hero* owner)
    : CombatArt<castarguments::Aim>(NULL, utils::Constants::LIGHTNING_COST, owner->aim()),
      hero_(owner)  {
    HudImageFactory factory;
    icon_ = factory.LightningIconImage();
}


bool HeroLightningWeapon::Available() const {
    return hero_->mana() >= cost_;
}

}
