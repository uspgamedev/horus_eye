#include "projectile.h"
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/graphic/image.h>
//#include "game/utils/circleobject.h"
#include "game/sprites/creatures/creature.h"

using namespace ugdk;
using namespace utils;

namespace sprite {

INITIALIZE_COLLIDABLE_NODE(Projectile, WorldObject);

Projectile::Projectile(int damage, float speed, int duration, Vector2D & dir) :
        direction_(Vector2D::Normalized(dir))
{
	damage_ = damage;
    speed_ = speed;
    duration_ = new TimeAccumulator(duration);
    exploding_ = false;
}

Projectile::~Projectile() {
    delete duration_;
}

void Projectile::Move(float delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void Projectile::Update(float delta_t) {
	if( duration_->Expired() ) {
	    this->status_ = WorldObject::STATUS_DEAD;
	}
	WorldObject::Update(delta_t);
	if (!exploding_)
	    this->Move(delta_t);
}

void Projectile::Explode() {
    if (!exploding_) {
        exploding_ = true;
        this->status_ = WorldObject::STATUS_DYING;
        duration_->Restart(250);
        set_visible(false);
    }
}

}
