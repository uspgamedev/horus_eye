#include <ugdk/time/timeaccumulator.h>

#include "projectile.h"

#include "game/sprites/creatures/creature.h"

using namespace ugdk;
using namespace utils;

namespace sprite {

Projectile::Projectile(double speed, int duration, Vector2D & dir) 
    :   TimedWorldObject(duration / 1000.0),
        speed_(speed),
        direction_(Vector2D::Normalized(dir))
    {}

void Projectile::Move(double delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void Projectile::Update(double delta_t) {
	super::Update(delta_t);
	if (is_active())
	    this->Move(delta_t);
}

}
