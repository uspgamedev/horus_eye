
#include <math.h>

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/entities/sandstormemitter.h"

#include "game/scenes/world.h"
#include "game/utils/settings.h"
#include "game/skills/usearguments.h"
#include "game/builders/projectilebuilder.h"

namespace entities {

using ugdk::Engine;
using ugdk::time::TimeAccumulator;

using builder::ProjectileBuilder;
using scene::World;
using utils::Settings;
using skills::usearguments::Aim;
using utils::Constants;

void SandstormEmitter::Update(double dt) {
    super::Update(dt);

    if(!suspended_ && projectile_interval_->Expired() ) {
        // resets the projectile cooldown timer.
        projectile_interval_->Restart();

        // emmits the projectiles.

        World *world = WORLD();
        // this is the direction versor and his normal vector,
        // we'll produce two spread vectors and reflect along this one.
        Vector2D aim_versor = (aim_.destination_ - aim_.origin_).Normalize();
        Vector2D aim_ortogonal = Vector2D(-aim_versor.y, aim_versor.x);
        // these are the actual directions the projectiles will be fired at.
        Vector2D versor[4];

        versor[0] =
            aim_ortogonal * sin(Constants::SANDSTORM_ANGLE/2.0)
            + aim_versor  * cos(Constants::SANDSTORM_ANGLE/2.0);
        versor[1] =
            aim_ortogonal * sin(Constants::SANDSTORM_ANGLE/6.0)
            + aim_versor  * cos(Constants::SANDSTORM_ANGLE/6.0);
        versor[2] =
            aim_ortogonal * -sin(Constants::SANDSTORM_ANGLE/6.0)
            + aim_versor  *  cos(Constants::SANDSTORM_ANGLE/6.0);
        versor[3] =
            aim_ortogonal * -sin(Constants::SANDSTORM_ANGLE/2.0)
            + aim_versor  *  cos(Constants::SANDSTORM_ANGLE/2.0);
        for(int i=0;i<4;i++) {
            ProjectileBuilder proj(world->image_factory());
            world->AddWorldObject(proj.Fireball(versor[i]), aim_.origin_);
            // TODO:FIREBALL!?
        }

        if(Settings::reference()->sound_effects())
            Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
    }
}

} // namespace entitites
