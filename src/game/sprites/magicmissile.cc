#include "magicmissile.h"
#include "mummy.h"
#include <ugdk/time/timeaccumulator.h>
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"
#include "../scenes/world.h"

using namespace ugdk;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT

namespace sprite {

INITIALIZE_COLLIDABLE_NODE(MagicMissile, Projectile);

MagicMissile::MagicMissile(Vector2D & dir) :
        Projectile(Constants::PROJECTILE_DAMAGE, Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir)
{
    ImageFactory *image_factory = WORLD()->image_factory();
    Initialize( image_factory->MagicMissileImage() );
    set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
    bound_ = new CircleObject(0.15f);
	set_light_radius(1.0f);
	known_collisions_[Mummy::Collision()] = new Collisions::Damage(this);
}

}
