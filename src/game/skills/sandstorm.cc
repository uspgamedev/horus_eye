
#include <ugdk/base/engine.h>

#include "game/skills/sandstorm.h"

#include "game/scenes/world.h"
#include "game/utils/settings.h"
#include "game/entities/sandstormemitter.h"
#include "game/sprites/creatures/creature.h"

namespace skills {

using ugdk::Engine;
using scene::World;
using utils::Constants;
using entities::SandstormEmitter;
using usearguments::Aim;

    Sandstorm::Sandstorm(sprite::Creature* owner) 
  : CombatArt<Aim>(NULL, Constants::SANDSTORM_COST, owner->mana(), owner->aim()),
    emitter_(NULL),
    maintain_mana_cost_(Constants::SANDSTORM_MAINTAIN_COST) {}
Sandstorm::~Sandstorm() {
    if(emitter_) delete emitter_;
}

void Sandstorm::Use() {
    if(!emitter_) {
        super::Use();

        emitter_ = new SandstormEmitter(use_argument_, &emitter_);

        World *world = WORLD();
        world->AddWorldObject(emitter_, use_argument_.origin_);

    } else {
        caster_mana_ -= maintain_mana_cost_;
        emitter_->Renew();
    }
}

} // namespace skills
