#include "game/entities/sandstormemitter.h"

#include <cmath>

#include <ugdk/math/integer2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/map/room.h"
#include "game/utils/settings.h"
#include "game/skills/usearguments.h"
#include "game/builders/projectilebuilder.h"

namespace entities {

using ugdk::Engine;
using ugdk::time::TimeAccumulator;
using ugdk::Vector2D;

using builder::ProjectileBuilder;
using utils::Settings;
using skills::usearguments::Aim;

void SandstormEmitter::Update(double dt) {
    super::Update(dt);

    if(!suspended_ && projectile_interval_->Expired() ) {
        // resets the projectile cooldown timer.
        projectile_interval_->Restart();

        // emmits the projectiles.

        // this is the direction versor and his normal vector,
        // we'll produce two spread vectors and reflect along this one.
        Vector2D aim_versor = (aim_.destination_ - aim_.origin_).Normalize();
        Vector2D aim_ortogonal = Vector2D(-aim_versor.y, aim_versor.x);
        // these are the actual directions the projectiles will be fired at.
        Vector2D versor[4];

        versor[0] =
            aim_ortogonal * sin(constants::GetDouble("SANDSTORM_ANGLE")/2.0)
            + aim_versor  * cos(constants::GetDouble("SANDSTORM_ANGLE")/2.0);
        versor[1] =
            aim_ortogonal * sin(constants::GetDouble("SANDSTORM_ANGLE")/6.0)
            + aim_versor  * cos(constants::GetDouble("SANDSTORM_ANGLE")/6.0);
        versor[2] =
            aim_ortogonal * -sin(constants::GetDouble("SANDSTORM_ANGLE")/6.0)
            + aim_versor  *  cos(constants::GetDouble("SANDSTORM_ANGLE")/6.0);
        versor[3] =
            aim_ortogonal * -sin(constants::GetDouble("SANDSTORM_ANGLE")/2.0)
            + aim_versor  *  cos(constants::GetDouble("SANDSTORM_ANGLE")/2.0);
        
        ProjectileBuilder proj;
        for(int i=0;i<4;i++) {
            current_room()->AddObject(proj.MagicBall(versor[i]), aim_.origin_, map::POSITION_ABSOLUTE);
            // TODO:FIREBALL!?
        }

        if(Settings::reference()->sound_effects())
            Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
    }
}

} // namespace entitites
