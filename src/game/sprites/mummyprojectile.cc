#include "mummyprojectile.h"
#include "hero.h"
#include "../../framework/engine.h"
#include "../../framework/timeaccumulator.h"
#include "../../framework/videomanager.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

using namespace framework;
using namespace scene;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT

namespace sprite {

MummyProjectile::MummyProjectile(Vector2D & dir, int damage) :
        Projectile(damage, Constants::PROJECTILE_SPEED,
                        Constants::PROJECTILE_DURATION, dir)
{
    ImageFactory *image_factory = WORLD()->image_factory();
    Initialize( image_factory->MummyProjectileImage() );
	set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );

    this->bound_ = new CircleObject(0.15f);
}

MummyProjectile::~MummyProjectile() {}

void MummyProjectile::CollidesWith(Hero *obj) { Explode(); }

void MummyProjectile::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
