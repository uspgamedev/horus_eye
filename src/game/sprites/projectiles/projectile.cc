#include <ugdk/time/timeaccumulator.h>

#include "projectile.h"

#include "game/sprites/creatures/creature.h"

using namespace ugdk;
using namespace utils;

namespace sprite {

Projectile::Projectile(float speed, int duration, Vector2D & dir) 
    :   TimedWorldObject(duration / 1000.0f),
        speed_(speed),
        direction_(Vector2D::Normalized(dir))
    {}

void Projectile::Move(float delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void Projectile::Update(float delta_t) {
	super::Update(delta_t);
	if (is_active())
	    this->Move(delta_t);
}

}
