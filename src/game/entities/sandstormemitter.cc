
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/entities/sandstormemitter.h"

#include "game/scenes/world.h"
#include "game/utils/settings.h"
#include "game/skills/usearguments.h"
#include "game/builders/projectilebuilder.h"

namespace entities {

using ugdk::Engine;
using ugdk::TimeAccumulator;

using builder::ProjectileBuilder;
using scene::World;
using utils::Settings;
using skills::usearguments::Aim;
using utils::Constants;

void SandstormEmitter::Update(float dt) {
    super::Update(dt);

    if(!suspended_ && projectile_interval_->Expired() ) {
        // reseta o timer
        projectile_interval_->Restart();

        // emite as bullets
        World *world = WORLD();
        Vector2D versor = (aim_.destination_ - aim_.origin_).Normalize();
        ProjectileBuilder proj(world->image_factory());
        world->AddWorldObject(proj.Fireball(versor), aim_.origin_);
        // TODO:FIREBALL!?

        if(Settings::reference()->sound_effects())
            Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
    }
}

} // namespace entitites