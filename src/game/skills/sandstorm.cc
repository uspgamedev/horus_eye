
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/image.h>

#include "game/skills/sandstorm.h"

#include "game/scenes/world.h"
#include "game/utils/settings.h"
#include "game/entities/sandstormemitter.h"
#include "game/sprites/creatures/hero.h"

namespace skills {

using ugdk::Engine;
using ugdk::Image;

using scene::World;
using utils::Settings;
using utils::Constants;
using entities::SandstormEmitter;
using usearguments::Aim;
using sprite::Hero;

Sandstorm::Sandstorm(Hero* owner) 
  : CombatArt<Aim>(NULL, Constants::SANDSTORM_COST, owner->mana(), owner->aim()),
    maintain_mana_cost_(Constants::SANDSTORM_MAINTAIN_COST),
    emitter_(NULL) {}
Sandstorm::~Sandstorm() {
    if(emitter_) delete emitter_;
}

void Sandstorm::Use() {
    if(!emitter_) {
        super::Use();

        emitter_ = new SandstormEmitter(use_argument_, &emitter_);
        emitter_->Initialize(new Image(), NULL, true);
        emitter_->set_visible(false);

        World *world = WORLD();
        world->AddWorldObject(emitter_, use_argument_.origin_);

        if(Settings::reference()->sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();

    } else {
        caster_mana_ -= maintain_mana_cost_;
        emitter_->Renew();
    }
}

} // namespace skills